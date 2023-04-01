/*
 * @Author: zzzzztw
 * @Date: 2023-03-30 09:16:06
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-30 15:00:07
 * @FilePath: /cpptest/server_copy.cpp
 */
#include "./server.h"

using namespace std;
Server::Server(int port, bool openLinger):epoller_(new Epoller()),port_(port),openLinger_(openLinger)
{
    listenFD_ = -1;

    listenEvent_ = EPOLLRDHUP|EPOLLET;
    connEvent_ = EPOLLONESHOT|EPOLLRDHUP|EPOLLET; 
    
}

Server::~Server(){
    close(listenFD_);
}



bool Server::InitSocket(){

    int ret;

    struct sockaddr_in addr;
    if(port_ > 65535 || port_ < 0){
        return false;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port_);

    struct linger optlinger = {0,0};

    if(openLinger_){
        /*socket如果阻塞，close()将等待l_linger这么久的时间让tcp缓冲区继续发送消息，
         *之后根据返回值和errno判断有无发送完全
         *如果非阻塞，close()将立即返回，根据返回值和errno判断数据发送完全
         */
        optlinger.l_onoff = 1;//开启 非0,关闭 0
        optlinger.l_linger = 1;
        
    }

    listenFD_ = socket(AF_INET,SOCK_STREAM,0);

    if(listenFD_ < 0){
        
        return false;
    }


    /*设置优雅关闭*/

    ret = setsockopt(listenFD_, SOL_SOCKET, SO_LINGER, &optlinger, sizeof(optlinger));
    
    if(ret < 0){
        /*log*/
        close(listenFD_);
        return false;
    }


    /*设置端口复用*/
    int optval = 1;
    ret = setsockopt(listenFD_, SOL_SOCKET, SO_REUSEADDR,&optval, sizeof(optval));

    if(ret < 0){
        close(listenFD_);
        return false;
    }


    ret = bind(listenFD_, (const sockaddr*)&addr, sizeof(addr));


    if(ret < 0){

        close(listenFD_);
        return false;
    }

    ret = listen(listenFD_, 10);

    if(ret < 0){

        close(listenFD_);
        return false;
    }


    ret = epoller_->AddFd(listenFD_, listenEvent_|EPOLLIN);
    if(ret == 0){
        close(listenFD_);
        return false;
    }

    ret = SetNoBlock(listenFD_);
    if(ret < 0){
        close(listenFD_);
        return false;
    }

    return true;
}

int Server::SetNoBlock(int fd){
    assert(fd > 0);
    int flag  = fcntl(fd,F_GETFD,0);
    flag |= O_NONBLOCK;
    
    return fcntl(fd,F_SETFL, flag);
}

void Server::AddClient_(int fd, sockaddr_in addr){
    assert(fd > 0);

    epoller_->AddFd(fd,connEvent_|EPOLLIN);
    SetNoBlock(fd);
    
}

/*获取套接字*/
void Server::DealListen_(){

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    do{

        int fd = accept(listenFD_, (struct sockaddr*)&addr, &len);
        if(fd <= 0){
            return;
        }
        AddClient_(fd,addr);

    }while(listenEvent_ & EPOLLET);

}




void Server::initCommand(){

    commands_.insert({"set", &Server::setCommand});
    commands_.insert({"get", &Server::getCommand});
    commands_.insert({"del", &Server::delCommand});
    commands_.insert({"load", &Server::loadCommand});
    commands_.insert({"dump", &Server::dumpCommand});
    commands_.insert({"login", &Server::loginCommand});
    commands_.insert({"quit", &Server::quitCommand});

}


bool Server::addUser(const std::string & username){
    userToClient_.insert({username,false});
    string filename = username + ".txt";
    ofstream file(filename);

    if(file.is_open()){
        /*文件创建成功*/
        file.close();
        return true;
    }
    else{
        /*文件创建失败*/
        return false;
    }
}

bool Server ::loguser(const std::string& username){
    if(userToClient_.count(username) == 0){
        return false;
    }
    std::unique_ptr<Client>client(new Client());
    client->username_ = username;
    std::unique_ptr<mydb>db(new mydb());
    client->db = db.get();

    userToDB_.emplace(username,std::move(db));

    return true;
}


void worker(){
    
}