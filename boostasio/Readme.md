<!--
 * @Author: zzzzztw
 * @Date: 2023-05-30 18:40:16
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-05 14:04:47
 * @FilePath: /myLearning/boostasio/Readme.md
-->
# 学习boost::asio网络库

### 需要提前配置boost::asio库到系统目录

## 同步sync基础Api

* endpoint:绑定一个对端地址（ip, port）

```cpp
//服务端：
unsigned short port = 1234;
boost::asio::ip::tcp::endpoint(asio::ip::address::v4(), port);

//或者
unsigned short port_num = 3333;
asio::ip::address ip_adress = asio::ip::address_v4::any();
asio::ip::tcp::endpoint ep(ip_adress, port_num);

//客户端

string addr = "127.0.0.1";
unsigned short port = 1234;
boost::system::error_code ec;
boost::asio::ip::tcp::endpoint(asio::ip::address::from_string(addr, ec), port);

```

* 创建用于通信的socket

```cpp
//服务端：直接使用简便方式创建上下文 + acceptor
boost::io_context ios;
boost::ip::tcp::acceptor _acceptor(ios, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1234));

// 第二种方式
int accept_new_connection(){
    const int BLACKLOG_SIZE = 30;
    unsigned short port_num = 3333;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
    asio::io_context ios;

    try{
        //创建一个连接器
        asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
        // 绑定我们需要对外开放的服务器终端地址
        acceptor.bind(ep);
        //监听这个地址
        acceptor.listen(BLACKLOG_SIZE);

        //建立连接返回一个新的socket，用于通信。
        asio::ip::tcp::socket sock(ios);
        acceptor.accept(sock);

    }catch(boost::system::system_error& e){
        std::cout<<"error code: "<<e.code()<<" message: "<<e.what();
        return e.code().value(); 
    }
}

//客户端
int connect_to_end(){

    // 1. 创建连接的端口号和ip地址
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;

    try{
        //2. 创建连接目标端点
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        
        //3. 创建sock并绑定bind端口
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        
        //4. 与对端建立连接
        sock.connect(ep);
    }catch(boost::system::system_error& e){
        std::cout<<"error code: "<<e.code()<<" message: "<<e.what();
        return e.code().value(); 
    }

    return 0;

}

```

* 用于接收消息的buffer缓冲

```cpp
//asio网络库中使用buffer麻烦的方式
void use_const_buffer(){
    std::string buf = "Hello world";
    asio::const_buffer asio_buf(buf.c_str(), buf.size());
    std::vector<asio::const_buffer>buffers_seq;
    buffers_seq.push_back(asio_buf); // 最终的buffer_seq就是可以传递给send的类型， 可以直接使用buffer转换为send需要的类型。
}

// 简单的方式
void use_const_str(){
    asio::const_buffers_1 output_buf = asio::buffer("hello, world");
}
// 字符串数组
void use_buffer_array(){
    const size_t BUF_SIZE_BYTES = 20;
    std::unique_ptr<char[]>buf(new char[BUF_SIZE_BYTES]);
    auto input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
}


//使用streanbuf，将输入输出流和streambuf绑定，可以实现流式输入和输出
void use_stream_buffer(){

    asio::streambuf buf;

    std::ostream output(&buf);

    output<<"Message\nMessage2";

    std::istream input(&buf);

    std::string message1;

    std::getline(input, message1);

}

```

* 发送与接收消息
  
```cpp
//同步写write_some，一点一点发，要一直绑定回调函数，直到发送指定的长度,注意回调时需要偏移一个已经发送的长度
//将buf中的数据发送给sock， 一点一点发，每次发了多少会返回给用户端
void write_to_socket(asio::ip::tcp::socket& sock){

    std::string buf = "hello world";
    std::size_t total_bytes_written = 0;

    // 循环发送
    //wirte_som 返回每次写入的字节数
    while(total_bytes_written != buf.size()){
        total_bytes_written += sock.write_some(asio::buffer(buf.c_str() + total_bytes_written, buf.size() - total_bytes_written));
    }
}


//同步send，一步全部发
//write_to_socket(sock); // 一点一点发送
/*
    一次性全发送完，发送完再通知
    阻塞到全发送完，返回发送的字节数，返回值小于等于0就说明出现了错误
*/
std::string buf = "hello world";
int sendlength = sock.send(asio::buffer(asio::buffer(buf.c_str(), buf.size())));
if(sendlength <=0)return 0;


//同步读read_some
std::string read_from_socket(asio::ip::tcp::socket& sock){
    const unsigned short MESSAGE_SIZE = 7;
    char buf[MESSAGE_SIZE];
    std::size_t total_bytes_read = 0;
    while(total_bytes_read != MESSAGE_SIZE){
        total_bytes_read += sock.read_some(asio::buffer(buf + total_bytes_read, MESSAGE_SIZE - total_bytes_read));
    }

    return std::string(buf, total_bytes_read);
}

//一次性全部读完receive
const size_t BUF_SIZE_BYTES = 20;
std::unique_ptr<char[]>buf(new char[BUF_SIZE_BYTES]);
auto input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
int length = sock.receive(input_buf);
if(length <= 0)return 0;

```

## 异步asyncApi

* 一般异步发送会创建一个结构体msgnode用于消息的传递
*  一个shared_ptr接收用于通信的socket，防止内存泄漏
*  一个shared_ptr封装消息体，用于构建待发送的msgnode结构体
* 一个发送队列，表示待发送的消息，防止多条消息乱发
* 发送标志位，true表示还有消息没发完
* 接收标志位，true表示还没读完

* 异步发送：
```cpp
// 使用api 封装消息结构体，调用async_write_some，注册回调函数，递归发送，直到发送完
// 1.async_write_some
void Session::WriteToSocketErr(const string buf){
    _send_node = std::make_shared<MsgNode>(buf.c_str(), buf.size());
    _sock->async_write_some(boost::asio::buffer(_send_node->_msg, _send_node->_total_len),
        std::bind(&Session::WriteCallBackErr,this,std::placeholders::_1, std::placeholders::_2, _send_node));
}

void Session::WriteCallBackErr(const boost::system::error_code& ec, std::size_t bytes_transferred, 
        std::shared_ptr<MsgNode> msg_node)
{
//当前发送的长度 + 已经发送的长度 小于总长度就说明需要继续发送
   if(bytes_transferred + msg_node->_cur_len < msg_node->_total_len){
        _send_node->_cur_len += bytes_transferred;
        _sock->async_write_some(asio::buffer(_send_node->_msg + _send_node->_cur_len, _send_node->_total_len - _send_node->_cur_len), 
            std::bind(&Session::WriteCallBackErr, this, std::placeholders::_1, std::placeholders::_2, _send_node));
   }
}
// 2.async_send, 内部封装async_write_some, 会直接把长度全部发完，和消息队列配合，一个完整信息发完了再发下一个。
void Session::WriteAllToSocket(const std::string buf){
    _send_queue.emplace(new MsgNode(buf.c_str(), buf.size()));

    if(_send_pending){
        return;
    }
    //async_send会重复调用async_write_some函数，直到把所有长度发完
    _sock->async_send(asio::buffer(buf), std::bind(&Session::WriteAllCallBack, this, std::placeholders::_1, std::placeholders::_2));
    _send_pending = true;
}

void Session::WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred){

    if(ec.value() != 0){
        std::cout<<"err !, error code is "<<ec.value() <<" message is "<<ec.message()<<endl;
        return;
    }

    _send_queue.pop();

    if(_send_queue.empty()){
        _send_pending = false;
    }

    if(!_send_queue.empty()){
        auto &send_data = _send_queue.front();
        _sock->async_write_some(asio::buffer(send_data->_msg, send_data->_total_len), std::bind(&Session::WriteAllCallBack, this, std::placeholders::_1, 
            std::placeholders::_2));
    }

}

```
* 异步读：
```cpp
// 一般使用async_read_some 一点一点读出来，过程更可控

// 利用async_read_some异步读
void Session::ReadFromSocket(){
    if(_recv_pending){//正在接收，直接返回
        return;
    }

    _recv_node = std::make_shared<MsgNode>(RECVSIZE);

    _sock->async_read_some(asio::buffer(_recv_node->_msg, _recv_node->_total_len), std::bind(&Session::ReadCallBack, this, std::placeholders::_1, std::placeholders::_2));
    _recv_pending = true;
}
void Session::ReadCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred){

    _recv_node ->_cur_len += bytes_transferred;
    if(_recv_node->_cur_len < _recv_node->_total_len){
        _sock->async_read_some(asio::buffer(_recv_node->_msg + _recv_node->_cur_len, _recv_node->_total_len - _recv_node->_cur_len),
            std::bind(&Session::ReadCallBack, this, std::placeholders::_1, std::placeholders::_2));
            return;
    }
    _recv_pending = true;
}

```

* 整个文件结构

```cpp
//封装一个Node结构，用来管理要发送和接收的数据，该结构包含数据域首地址，数据的总长度，以及已经处理的长度(已读的长度或者已写的长度)
class MsgNode{
public:
    MsgNode(const char*msg, int total_len):_total_len(total_len), _cur_len(0){
        _msg = new char[_total_len];
        memcpy(_msg, msg, _total_len);
    }
    MsgNode(int total_len):_total_len(total_len), _cur_len(0){
        _msg = new char[_total_len];
    }
    ~MsgNode(){
        delete[] _msg;
    }
    int _total_len;
    int _cur_len;
    char* _msg;

};

//处理会话类
class Session{
public:
    Session(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void Connect(const boost::asio::ip::tcp::endpoint& ep);
    
    //正确姿势是采用消息队列保证时序性，发送完一个消息再发下一个
    void WriteCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
    void WriteToSocket(const std::string buf);

    //常用，async_send(), 其内部封装了async_write_some,多次调用，直到把长度发完
    void WriteAllToSocket(const std::string buf);
    void WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);


    //异步读操作,会多次递归调用async_read_some, 一般用这个，效率高点，可控。
    void ReadFromSocket();
    void ReadCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);

    //异步读操作，一次全部读出，async_receive内部封装async_read_some
    void ReadAllFromSocket();
    void ReadaLLCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
private:

    std::shared_ptr<boost::asio::ip::tcp::socket>_sock;
    std::shared_ptr<MsgNode>_send_node;
    //异步写
    std::queue<std::shared_ptr<MsgNode>>_send_queue;
    bool _send_pending;

    //异步读

    std::shared_ptr<MsgNode>_recv_node;
    bool _recv_pending;
};

```

# Async 官方案例：

1. Session会话类管理并处理新进来的连接, 根据其绑定的读或写回调函数执行处理。

```cpp

class Session{

public:
    Session(boost::asio::io_context& ioc):_socket(ioc){

    }
    tcp::socket& Socket(){
        return _socket;
    }
    // 监听对服务器的读和写
    void Start();


private:
    //读和写的回调函数。
    void handler_read(const boost::system::error_code& err, std::size_t bytes_transferred);
    void handler_write(const boost::system::error_code& err);
    tcp::socket _socket;
    //简单的固定一个长度
    enum {max_length = 1024};
    char _data[max_length];
};
oid Session::Start(){
    memset(_data, 0, max_length);
    // 绑定一个读事件，填进底层的epoll时间表，并将数据自动的读到buffer中，proactor的特性，异步处理
    _socket.async_read_some(boost::asio::buffer(_data, max_length), std::bind(&Session::handler_read, this,
        std::placeholders::_1, std::placeholders::_2));
}

void Session::handler_read(const boost::system::error_code& err, std::size_t bytes_transferred){

    if(!err){
        cout<<"server receice data is "<<_data<<endl;
        boost::asio::async_write(_socket, boost::asio::buffer(_data, bytes_transferred),
            std::bind(&Session::handler_write, this, std::placeholders::_1));
        

    }
    else{
        cout<<"read err! "<<err.value()<<endl;
        delete this; // 生产环境中不会这么做
    }
}

void Session::handler_write(const boost::system::error_code& err){
    if(!err){
        memset(_data, 0, sizeof _data);
        cout<<"server receive data is "<< _data<<endl;
        _socket.async_read_some(boost::asio::buffer(_data, sizeof _data), std::bind(&Session::handler_read, this, std::placeholders::_1,
        std::placeholders::_2));
    }else{
        cout<<"write err! "<<err.value()<<endl;
        delete this; // 生产环境中不会这么做
    }
}

```

2. Server类负责创建一个acceptor，相当于大堂经理，接收新进来的连接并交给Session根据注册的回调函数去处理连接。

```cpp
class Server{
public:
    Server(boost::asio::io_context& ioc, short port);

private:
    void start_accept();
    void handle_accept(Session* new_session, const boost::system::error_code& ec);

    boost::asio::io_context& _ioc; //私有，上下文不允许拷贝构造
    tcp::acceptor _acceptor;
};


Server::Server(boost::asio::io_context& ioc, short port):_ioc(ioc), _acceptor(ioc, tcp::endpoint(tcp::v4(), port))
{
    std::cout<<"Server start success on port"<<port<<std::endl;
    start_accept();
}

void Server::start_accept(){
    // session类似于服务人员，acceptor类似大堂经理，大堂经理把任务给服务人员，叫他去处理。
    Session* new_session = new Session(_ioc);
    _acceptor.async_accept(new_session->Socket(), std::bind(&Server::handle_accept, this, new_session, std::placeholders::_1));//进来一个连接，服务器使用newsession保存这个连接。
}

void Server::handle_accept(Session* new_session, const boost::system::error_code& ec){
    if(!ec){
        new_session->Start();
    }else{
        delete new_session;
    }
    // 处理完了，在创建一个startaccept 去处理新连接
    start_accept();
}


```