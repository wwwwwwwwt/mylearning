
#ifndef SESSION_H
#define SESSION_H
#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <queue>

const int RECVSIZE = 1024;

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

    //这两种方法的逻辑会导致第一个tcp层不是空的情况下，一个消息发了一半，在调用回调函数继续发送时，另一个消息先进行发送，导致出现“hello |hello world| world”的情况
    void WriteCallBackErr(const boost::system::error_code& ec, std::size_t bytes_transferred, 
        std::shared_ptr<MsgNode> send_node);
    void WriteToSocketErr(const std::string buf);

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



#endif 
