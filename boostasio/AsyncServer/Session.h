/*
 * @Author: zzzzztw
 * @Date: 2023-06-01 22:34:34
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-01 22:52:10
 * @FilePath: /myLearning/boostasio/AsyncServer/Session.h
 */
#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session{

public:
    Session(boost::asio::io_context& ioc):_socket(ioc){

    }
    tcp::socket& Socket(){
        return _socket;
    }

    void Start();


private:
    void handler_read(const boost::system::error_code& err, std::size_t bytes_transferred);
    void handler_write(const boost::system::error_code& err);
    tcp::socket _socket;
    enum {max_length = 1024};
    char _data[max_length];
};


#endif 

