#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

const int BUFFER_SIZE = 1024;
const int MAX_EVENTS = 100;

class KVDatabase {
public:
    KVDatabase() {
        load();
    }

    void put(int fd, const string& key, const string& value) {
        lock_guard<mutex> lock(mutex_);
        data_[fd][key] = value;
        send_response(fd, "OK\n");
    }

    void get(int fd, const string& key) {
        lock_guard<mutex> lock(mutex_);
        auto it = data_[fd].find(key);
        if (it != data_[fd].end()) {
            send_response(fd, it->second + "\n");
        } else {
            send_response(fd, "NOT_FOUND\n");
        }
    }

    void remove(int fd, const string& key) {
        lock_guard<mutex> lock(mutex_);
        auto it = data_[fd].find(key);
        if (it != data_[fd].end()) {
            data_[fd].erase(it);
            send_response(fd, "OK\n");
        } else {
            send_response(fd, "NOT_FOUND\n");
        }
    }

    void dump(int fd) {
        thread t(&KVDatabase::dump_thread, this, fd);
        t.detach();
    }

    void load(int fd) {
        thread t(&KVDatabase::load_thread, this, fd);
        t.detach();
    }

private:
    void dump_thread(int fd) {
        lock_guard<mutex> lock(mutex_);
        FILE* file = fopen(get_filename(fd).c_str(), "w");
        if (file) {
            for (const auto& p : data_[fd]) {
                fprintf(file, "%s %s\n", p.first.c_str(), p.second.c_str());
            }
            fclose(file);
            send_response(fd, "OK\n");
        } else {
            send_response(fd, "ERROR\n");
        }
    }

    void load_thread(int fd) {
        lock_guard<mutex> lock(mutex_);
        FILE* file = fopen(get_filename(fd).c_str(), "r");
        if (file) {
            char buffer[BUFFER_SIZE];
            while (fgets(buffer, BUFFER_SIZE, file)) {
                int len = strlen(buffer);
                if (buffer[len - 1] == '\n') {
                    buffer[len - 1] = '\0';
                }
                istringstream ss(buffer);
                string key, value;
                ss >> key >> value;
                data_[fd][key] = value;
            }
            fclose(file);
            send_response(fd, "OK\n");
        } else {
            send_response(fd, "ERROR\n");
        }
    }

    /*void load() {
        lock_guard<mutex> lock(mutex_);
        for (int fd = 3; fd < MAX_EVENTS; fd++) {
            FILE* file = fopen(get_filename(fd).c_str*/
    // 根据文件描述符获取对应的文件名
    string get_filename(int fd) const {
        return "db-" + to_string(fd) + ".txt";
    }

    // 发送响应给客户端
    void send_response(int fd, const string& response) {
        send(fd, response.c_str(), response.size(), 0);
    }

    private:
unordered_map<int, unordered_map<string, string>> data_;
mutex mutex_;
};

class EventLoop {
public:
EventLoop(int port) : port_(port) {}
void start() {
    // 创建监听 socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(1);
    }

    // 设置 socket 地址复用
    int optval = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // 绑定监听地址
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port_);
    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // 开始监听
    if (listen(listen_fd, SOMAXCONN) < 0) {
        perror("listen");
        exit(1);
    }

    // 创建 epoll 实例并添加监听 socket
    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        exit(1);
    }
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) < 0) {
        perror("epoll_ctl EPOLL_CTL_ADD");
        exit(1);
    }

    // 处理事件循环
    struct epoll_event events[MAX_EVENTS];
    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            if (fd == listen_fd) {
                // 监听 socket 有新连接请求
                struct sockaddr_in client_addr;
                socklen_t client_addrlen = sizeof(client_addr);
                int conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addrlen);
                if (conn_fd < 0) {
                    perror("accept");
                    continue;
                }
                // 将新连接的 socket 加入 epoll 实例
                struct epoll_event event;
                event.events = EPOLLIN;
                event.data.fd = conn_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &event) < 0) {
                    perror("epoll_ctl EPOLL_CTL_ADD");
                    exit(1);
                }
                // 初始化 KVDatabase 实例并将其与新连接的 socket 关联
                databases_[conn_fd] = make_shared<KVDatabase>();
             //   databases_[conn_fd]->load(conn_fd
                 // 启动子线程，定期进行数据 dump
                threads_[conn_fd] = thread([this, conn_fd]() {
                    while (true) {
                        this_thread::sleep_for(chrono::seconds(DUMP_INTERVAL));
                        databases_[conn_fd]->dump(conn_fd);
                    }
                });
            } else {
                // 已连接 socket 有数据可读
                shared_ptr<KVDatabase> database = databases_[fd];
                char buf[BUF_SIZE];
                int n = recv(fd, buf, BUF_SIZE, 0);
                if (n < 0) {
                    perror("recv");
                    continue;
                } else if (n == 0) {
                    // 对端关闭了连接
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                    threads_[fd].join();
                    databases_.erase(fd);
                    continue;
                }
                // 解析请求，执行对应操作
                string request(buf, n);
                vector<string> tokens = split(request);
                if (tokens.empty()) {
                    continue;
                }
                string command = tokens[0];
                if (command == "GET" && tokens.size() == 2) {
                    string key = tokens[1];
                    string value = database->get(key);
                    send_response(fd, value);
                } else if (command == "SET" && tokens.size() == 3) {
                    string key = tokens[1];
                    string value = tokens[2];
                    database->set(key, value);
                    send_response(fd, "OK\n");
                } else if (command == "DELETE" && tokens.size() == 2) {
                    string key = tokens[1];
                    database->remove(key);
                    send_response(fd, "OK\n");
                } else {
                    send_response(fd, "Invalid command\n");
                }
            }
        }
    }
}
private:
int port_;
unordered_map<int, shared_ptr<KVDatabase>> databases_;
unordered_map<int, thread> threads_;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
    cerr << "Usage: " << argv[0] << " PORT" << endl;
    return 1;
    }
    int port = atoi(argv[1]);
    if (port <= 0) {
    cerr << "Invalid port number" << endl;
    return 1;
    }
    EventLoop event_loop(port);
    event_loop.start();
    return 0;
}

/*
以上代码可以实现在主线程为单线程 io 端口多路复用，由子线程进行 dump 和 load 操作。在程序运行时，主线程会创建一个 epoll 文件描述符并监听指定端口，当有新的客户端连接时，主线程会创建一个对应的子线程并为其分配一个数据库实例，该子线程会定期执行 dump 操作；当有客户端请求到达时，主线程会将请求解析后调用对应的数据库方法执行，并将结果发送回客户端。

当客户端关闭连接时，主线程会在 epoll 中删除对应的文件描述符并等待对应的子线程退出，最后将该连接对应的数据库实例和子线程从哈希表中删除。

需要注意的是，在上述代码中，主线程并没有显式地将子线程设置为 detached，因此子线程在退出时需要通过调用 join 来等待线程结束，否则会导致程序崩溃。


*/


/*

在上述代码中，对于每一个连接，都会创建一个对应的子线程，该子线程会定期执行 dump 操作。

因此，如果有多个客户端连接，那么就会创建多个对应的子线程。在这种情况下，每个连接都会有一个独立的子线程执行 dump 操作，这些子线程之间是相互独立的，互不干扰。

需要注意的是，子线程的创建和销毁是有一定开销的，如果连接数过多，可能会导致系统负载过高，影响程序性能。因此，在实际应用中，应该合理设置定期 dump 的时间间隔，避免过于频繁的操作。

*/