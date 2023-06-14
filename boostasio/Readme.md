<!--
 * @Author: zzzzztw
 * @Date: 2023-05-30 18:40:16
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-14 18:06:13
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
# 改进官网案例，利用智能指针模拟闭包延长Session会话生命周期 + 增加一个发送队列 （AsyncServer/async01）

1. 为什么要延长Session会话声明周期？
答：极端情况，客户端关闭，服务逻辑同时触发了读回调和写回调函数，导致进入错误逻辑判断时，重复删除一个会话Session类的指针，造成二次析构的问题

2. 解决方案，通过智能指针管理Session类，将acceptor接收的新的链接保存在Session类型的智能指针类中。由于智能指针引用计数为0时，自动析构，我们将其放入一个map中，自动将其计数+1，所以当我们将其从map中删除时，其计数才会为0.
3. map的key为这个连接的uuid，val为这个连接的智能指针

```cpp
class CServer
{
public:
    CServer(boost::asio::io_context& io_context, short port);
    void ClearSession(std::string);
private:
    void HandleAccept(shared_ptr<CSession>, const boost::system::error_code & error);
    void StartAccept();
    boost::asio::io_context &_io_context;
    short _port;
    tcp::acceptor _acceptor;
    std::map<std::string, shared_ptr<CSession>> _sessions;
};
```

4. 处理链接:虽然是一个局部变量，但通过bind操作，将其以值传递给bind，计数+1，保证了其不会释放。在处理连接的回调函数中，处理链接并将uuid加入map，保证我们erase前智能指针不会被释放

```cpp
void CServer::StartAccept() {
    shared_ptr<CSession> new_session = make_shared<CSession>(_io_context, this);
    _acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, placeholders::_1));

}


void CServer::HandleAccept(shared_ptr<CSession> new_session, const boost::system::error_code& error){
    if (!error) {
        new_session->Start();
        _sessions.insert(make_pair(new_session->GetUuid(), new_session));
    }
    else {
        cout << "session accept failed, error is " << error.what() << endl;
    }
    StartAccept();
}
```

5. 构造伪闭包的思路

* 利用智能指针被复制或使用引用计数加一的原理保证内存不被回收
*  bind操作可以将值绑定在一个函数对象上生成新的函数对象，如果将智能指针作为参数绑定给函数对象，那么智能指针就以值的方式被新函数对象使用，那么智能指针的生命周期将和新生成的函数对象一致，从而达到延长生命的效果。

6. 为什么要增加一个发送队列
答：全双工通信方式，服务器一直监听写事件，接收对端数据，可随时发送数据给对端，因为多次发送时，异步的发送要保证回调触发后再次发送才能确保数据是有序的，

7. 具体做法：
   1. 设计数据节点存储数据
    ```cpp
    class MsgNode
    {
        friend class CSession;
    public:
        MsgNode(char * msg, int max_len) {
            _data = new char[max_len];
            memcpy(_data, msg, max_len);
        }
        ~MsgNode() {
            delete[] _data;
        }
    private:
        int _cur_len;
        int _max_len;
        char* _data;
    };
    1  _cur_len表示数据当前已处理的长度(已经发送的数据或者已经接收的数据长度)，因为一个数据包存在未发送完或者未接收完的情况。
    2  _max_len表示数据的总长度。
    3  _data表示数据域，已接收或者已发送的数据都放在此空间内。
    ```
    2. 封装发送接口
    ```cpp
    void CSession::Send(char* msg, int max_length) {
	
	std::lock_guard<std::mutex> lock(_send_lock);
	bool pending = false;
	if (_send_que.size() > 0) {
		pending = true;
	}
	_send_que.push(make_shared<MsgNode>(msg, max_length));
	if (pending) {
		return;
	}
	auto& msgnode = _send_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len), 
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));
    }
    void CSession::HandleWrite(const boost::system::error_code& error, shared_ptr<CSession> _self_shared) {
	if (!error) {
		std::lock_guard<std::mutex> lock(_send_lock);
		_send_que.pop();
		if (!_send_que.empty()) {
			auto &msgnode = _send_que.front();
			boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
				std::bind(&CSession::HandleWrite, this, std::placeholders::_1, _self_shared));
		}
	}
	else {
		std::cout << "handle write failed, error is " << error.value() << endl;
		Close();
		_server->ClearSession(_uuid);
	}
    }
   1. 首先在CSession类里新增一个队列存储要发送的数据，因为我们不能保证每次调用发送接口的时候上一次数据已经发送完，就要把要发送的数据放入队列中，通过回调函数不断地将队列中的消息发送完。
   2. 而且我们不能保证发送的接口和回调函数的接口在一个线程，所以要增加一个锁保证发送队列安全性。
   同时我们新增一个发送接口Send

    ```
# 切包思想处理粘包现象，消息长度 + 消息体 （AsyncServer/async02）
1. 到目前为止，框架的逻辑是CServer类管理新链接，核心是acceptor，每新到一个连接就创建一个新的CSession类，管理新的会话，并将这个会话的uuid和智能指针放进map，用于管理。
2. CSession类封装了一个msgnode用于管理发送或读入的消息，其有总长度和现在已经处理完的长度，还有一个data指针用于处理待处理的数据。CSession类管理了用于通信的socket文件描述符，这个连接的uuid， 接收或发送数据的缓冲区，处理读操作和写操作的回调函数（底层事件驱动，tcp缓冲区有无数据），异步发送队列
3. 粘包产生的原因：
   1. 因为TCP底层通信是面向字节流的，TCP只保证发送数据的准确性和顺序性，字节流以字节为单位，客户端每次发送N个字节给服务端，N取决于当前客户端的发送缓冲区是否有数据，比如发送缓冲区总大小为10个字节，当前有5个字节数据(上次要发送的数据比如’loveu’)未发送完，那么此时只有5个字节空闲空间，我们调用发送接口发送hello world！其实就是只能发送Hello给服务器，那么服务器一次性读取到的数据就很可能是loveuhello。而剩余的world！只能留给下一次发送，下一次服务器接收到的就是world！
   2.   客户端的发送频率远高于服务器的接收频率，就会导致数据在服务器的tcp接收缓冲区滞留形成粘连，比如客户端1s内连续发送了两个hello world！,服务器过了2s才接收数据，那一次性读出两个hello world！。
   3. tcp底层的安全和效率机制不允许字节数特别少的小包发送频率过高，tcp会在底层累计数据长度到一定大小才一起发送，比如连续发送1字节的数据要累计到多个字节才发送，可以了解下tcp底层的Nagle算法。
   4. 再就是我们提到的最简单的情况，发送端缓冲区有上次未发送完的数据或者接收端的缓冲区里有未取出的数据导致数据粘连。
4. 处理粘包：定义消息格式，消息长度 + 消息内容

```cpp
void CSession::HandleRead(const boost::system::error_code& error, size_t  bytes_transferred, shared_ptr<CSession> _self_shared){
	if(!error){
		int copy_len = 0;
		while(bytes_transferred > 0){
			if(!_b_head_parse){
				//当前收到的数据 加上本来的数据还没到头部长度，就注册回调函数，继续接收
				if(bytes_transferred + _recv_head_node->_cur_len < HEAD_LENGTH){
					memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, bytes_transferred);
					_recv_head_node->_cur_len += bytes_transferred;
					memset(_data, 0, sizeof _data);
					_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, 
					std :: placeholders::_1, std::placeholders::_2, _self_shared));
					return;
				}

				//当前收到的数据加上已有的数据比头部多了,填满head再把剩下的向body中填
				int head_remain = HEAD_LENGTH - _recv_head_node->_cur_len;
				memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, head_remain);
				copy_len += head_remain;
				bytes_transferred -= head_remain;

				//从头部获取这个消息中实际的数据长度
				short data_len = 0;
				memcpy(&data_len, _recv_head_node->_data, HEAD_LENGTH);
				cout<<"data_len is "<<data_len<<endl;

				//如果头部长度非法，直接关闭连接
				if(data_len > MAX_LENGTH){
					std::cout << "invalid data length is " << data_len << endl;
					_server->ClearSession(_uuid);
					return;		
				}

				//拿到头部的实际消息长度，开始处理消息体
				_recv_msg_node = std::make_shared<MsgNode>(data_len);
				//如果当前剩下的消息，还不够实际消息的全部长度，就全部填入消息体，然后接着读
				if(bytes_transferred < data_len){
					memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
					_recv_msg_node->_cur_len += bytes_transferred;
					memset(_data, 0, MAX_LENGTH);
					_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, 
					std::placeholders::_1, std::placeholders::_2, _self_shared));
					//走到这里逻辑是头部处理完成了，消息体还没满
					_b_head_parse = true;
					return;
				}

				// 否则，当前剩下的消息数据，满足这个包内容的实际长度，就将实际内容大小的消息长度，填充进消息体，然后就解析完这个包。重新将解析头标志位归false.
				// 如果剩下数据长度还有多余的，就continue，否则绑定回调函数，return
				memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, data_len);
				_recv_msg_node->_cur_len += data_len;
				copy_len += data_len;
				bytes_transferred -= data_len;
				_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
				cout<<"receive data is "<<_recv_msg_node->_data<<endl;

				// 拿到数据包后的逻辑处理，此处暂时发送回客户端
				Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
				
				// 继续轮询剩余的未处理数据
				_b_head_parse = false;
				_recv_head_node->Clear();
				if(bytes_transferred <= 0){
					memset(_data, 0, MAX_LENGTH);
					_socket.async_read_some(boost::asio::buffer(static_cast<void *>(_data), MAX_LENGTH), std::bind(&CSession::HandleRead, this,
					std::placeholders::_1, std::placeholders::_2, _self_shared));
					return;
				}
				// 还有剩余的，继续轮询处理下一个包的数据
				continue;
			}

			//如果这时候状态是已经处理完头部了，那么剩下的数据就要向内容里填
			int remain_msg = _recv_msg_node->_total_len - _recv_msg_node->_cur_len;
			//如果当前读到的，小于实际总的，就全填进去，注册读回调，接着读
			if(bytes_transferred < remain_msg){
				memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
				_recv_msg_node->_cur_len += bytes_transferred;
				memset(_data, 0, MAX_LENGTH);
				_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, 
				std::placeholders::_1, std::placeholders::_2, _self_shared));
				return;
			}
			memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, remain_msg);
			_recv_msg_node->_cur_len += remain_msg;
			bytes_transferred -= remain_msg;
			copy_len += remain_msg;
			_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
			cout<<"receive data is "<<_recv_msg_node->_data<<endl;

			// 否则，读到这个包所有数据，拿到数据包后的逻辑处理，此处暂时发送回客户端
			Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
			_recv_head_node->Clear();
			if(bytes_transferred <= 0){
				memset(_data, 0, MAX_LENGTH);
				_socket.async_read_some(boost::asio::buffer(static_cast<void *>(_data), MAX_LENGTH), std::bind(&CSession::HandleRead, this,
				std::placeholders::_1, std::placeholders::_2, _self_shared));
				return;
			}
			continue;
		}
	}

}
```

# 网络字节序

1. 网络字节序全部使用大端序，即数据的高位存储在低地址。
2. 只有整型需要字节序，浮点数或字符类型都不需要，原因是整型是由多个字节组合起来的。浮点数走另一套IEEE规定，字符UTF-8规定
3. 代码检测机器用的是大端序还是小端序，将十六进制数转换为char地址再取值，就能获得其存储后的低地址字节，即如果得到的字节是1，就是低位存储在低地址，小端序；否则，大端序。

```cpp
#include <iostream>
using namespace std;
bool is_big_endian(){
    int num = 0x00000001;
    cout<<hex<<(int)*((char*)&num)<<endl;
    if(*(char*)&num == 1){
        return false;
    }else return true;
}
int main(){

    int num = 0x12345678;
    char* p = (char*)&num;
    cout<<sizeof(num)<<endl;
    cout<<"原始数据 "<<hex<<num<<endl;
    for(int i = 0; i < sizeof(num);i++){
        cout<<hex<<(int)*(p + i)<<" ";
    }
    cout<<endl;
    if(is_big_endian()){
        cout<<"大端序"<<endl;
    }else cout<<"小端序"<<endl;

    return 0;
}
4
原始数据 12345678
78 56 34 12 
1
小端序
```

# 简单使用protobuf(async03)

* 编写proto文件
```shell
syntax="proto3";

message MsgData{
    int32 id = 1;
    string data = 2;
}
```
* 编译出cpp格式的pb文件
```shell
protoc --cpp_out=. ./msg.proto
—cpp_out= 表示指定要生成的pb文件所在的位置
./msg.proto 表示msg.proto所在的位置，因为我们是在msg.proto所在文件夹中执行的protoc命令,所以是当前路径即可。
```

* 修改发送逻辑（服务端收到后，进行解析，再序列化发给客户端）

```cpp
    MsgData msgdata;
    std::string receive_data;
    msgdata.ParseFromString(std::string(_recv_msg_node->_data, _recv_msg_node->_total_len));
    std::cout << "recevie msg id  is " << msgdata.id() << " msg data is " << msgdata.data() << endl;
    std::string return_str = "server has received msg, msg data is " + msgdata.data();
    MsgData msgreturn;
    msgreturn.set_id(msgdata.id());
    msgreturn.set_data(return_str);
    msgreturn.SerializeToString(&return_str);
    Send(return_str);
```

* 客户端发送消息逻辑（先创造pb的msg节点，向里面填充数据，再序列化）

```cpp
  MsgData msgdata;
  msgdata.set_id(1001);
  msgdata.set_data("hello world");
  std::string request;
  msgdata.SerializeToString(&request);
```
* 编译时，后面加上-lprotobuf

# 简单使用json进行消息传递(async04)

* demo
```cpp
#include <iostream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

int main(){
    Json::Value root;
    root["id"] = 1001;
    root["data"] = "hello world";
    std::string request = root.toStyledString();
    std::cout<<"request is "<<request<<std::endl;

    Json::Value root2;
    Json::Reader reader;
    reader.parse(request, root2);
    std::cout<<"msg id is "<<root2["id"] <<" msg is "<<root2["data"]<<std::endl;
    return 0;
}

```

```cpp
  Json::Reader reader;
  Json::Value root;
  reader.parse(std::string(_recv_msg_node->_data, _recv_msg_node->_total_len), root);
  std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is " << root["data"].asString() << endl;
  root["data"] = "server has received msg, msg data is " + root["data"].asString();
  std::string return_str = root.toStyledString();
```

# 使用单例模式构建单逻辑线程 + 逻辑队列处理回调函数 (async05)

* 增添single.h LogicSystem.h LogicSystem.cpp 文件


## 目前逻辑整理：

1. 网络层Server：
   1.  server端使用asio网络层，接收新进来的链接，io_context.run()死循环轮询，这个事件循环会在需要处理的异步操作完成时自动触发相关的回调函数，以及在没有待处理事件时休眠以节省系统资源。
   2.  使用一个新的CSession会话类对象来从acceptor中拿到这个会话的socket，然后start，之后由这个对象对这个链接进行服务

2. 会话层CSession：
   1. Csession进行start，异步的读取客户端发来的消息
   2. 进入handleread逻辑，按我们定义的消息id + 消息长度 + 消息内容的格式读取内容，防止粘包，并按json反序列化回来，处理网络字节序到本地字节序的转化。之后进入处理逻辑 + 组装回复的消息（可选）。
   3. 如果读好了，调用Send异步发送我们的消息（这里关联着逻辑队列）

3. 逻辑层：LogicSystem
   1. 继承于懒汉单例模式，使之只有一个逻辑线程来处理，避免共享资源访问频繁加锁导致的性能下降，如redis也采用这种思路。特别注意：由于基类继承于单例，所以自身也需要将构造函数私有化，但是单例类的构造出逻辑线程对象的智能指针GetInstance函数中调用了new T，T就是逻辑类本身，所以派生的逻辑类需要将单例类变为自己的友元类，这样单例类的才可以访问其派生类私有的构造函数
   2. 逻辑层的消息封装Logicnode 和 RecvNode 将LogicSystem设为自己友元类，这样LogicSystem可以使用其私有函数
   3. 逻辑层初始化调用注册函数，将我们设置的回调函数注册到哈希表，根据接收到的消息id从哈希表中取到对应的回调函数并调用。
   4. 使用单线程work_thread作为消费者，死循环执行DealMsg，从消息队列中取出消息，解析消息id并执行回调
   5. 会话层使用单例逻辑调用PostToque，作为生产者，将会话层解析后的消息id，消息内容打包放进消息队列。

## 优雅推出的两种方式：

1. 利用asio库提供的signal_set
* 利用signal_set 将信号SIGINT SIGTERM注册进io_context中，然后异步等待捕捉信号，捕捉到信号后就调用io_context.stop()，之后将执行析构所有对象的逻辑。
```cpp
#include <iostream>
#include "CSession.h"
#include "CServer.h"
#include <boost/asio.hpp>
#include <exception>
#include <signal.h>
using namespace std;
int main(){

    try{
        boost::asio::io_context ioc;
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](int , int){
            ioc.stop();
        });
        CServer s(ioc, 10086);
        // io_context io.run(), 开始轮询
        //启动异步事件循环。这个事件循环会在需要处理的异步操作完成时自动触发相关的回调函数，以及在没有待处理事件时休眠以节省系统资源。
        ioc.run();

    }catch(std::exception& e){
        std::cout<<"err ! err is "<<e.what()<<std::endl;
    }

}

```

2. c风格两个线程，一个主线程执行ioc.run, 一个子线程捕捉信号, 捕捉到后就通过信号量主线程

```cpp
std::condition_variable cv_;
std::mutex mtx_;
volatile bool stop_ = false;
void signal_handler(int sig){
    if(sig == SIGINT || sig == SIGTERM){
        unique_lock<mutex>lokcer(mtx_);
        stop_ = true;
        cv_.notify_all();
    }
}

int main(){
    try{
        boost::asio::io_context ioc;
        std::thread t([&](){
            CServer s(ioc, 10086);
            ioc.run();
        });
        signal(SIGINT,signal_handler);
        signal(SIGTERM, signal_handler);
        unique_lock<mutex>locker(mtx_);
        cv_.wait(locker, [&]{
            return stop_ == true;
        });
        ioc.stop();
        t.join();

    }catch(std::exception& e){
        std::cout<<"err! err is "<<e.what()<<std::endl;
    }
}
```