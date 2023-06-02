/*
 * @Author: zzzzztw
 * @Date: 2023-06-01 22:34:34
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-02 12:47:01
 * @FilePath: /myLearning/boostasio/AsyncServer/Session.h
 */
#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

//管理会话
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


class Server{
public:
    Server(boost::asio::io_context& ioc, short port);

private:
    void start_accept();
    void handle_accept(Session* new_session, const boost::system::error_code& ec);

    boost::asio::io_context& _ioc; //私有，上下文不允许拷贝构造
    tcp::acceptor _acceptor;
};

#endif 


