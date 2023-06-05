/*
 * @Author: zzzzztw
 * @Date: 2023-06-01 22:34:34
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-05 19:37:20
 * @FilePath: /myLearning/boostasio/AsyncServer/Session.h
 */
#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <boost/asio.hpp>
#include <map>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using boost::asio::ip::tcp;

class Server;
//管理会话
class Session: public std::enable_shared_from_this<Session>
{

public:
    Session(boost::asio::io_context& ioc, Server* server):_socket(ioc), _server(server){
        boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
        _uuid = boost::uuids::to_string(a_uuid);
    }
    tcp::socket& Socket(){
        return _socket;
    }
    // 监听对服务器的读和写
    void Start();

    std::string& GetUUid(){
        return _uuid;
    }
    

private:
    //读和写的回调函数。
    void handler_read(const boost::system::error_code& err, std::size_t bytes_transferred, std::shared_ptr<Session>_self_ptr);
    void handler_write(const boost::system::error_code& err, std::shared_ptr<Session>_self_ptr);
    tcp::socket _socket;
    //简单的固定一个长度
    enum {max_length = 1024};
    char _data[max_length];
    Server* _server;
    std::string _uuid;
};


class Server{
public:
    Server(boost::asio::io_context& ioc, short port);
    void ClearSession(std::string uuid);
    
private:
    void start_accept();
    void handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& ec);

    boost::asio::io_context& _ioc; //私有，上下文不允许拷贝构造
    tcp::acceptor _acceptor;
    

    std::map<std::string, std::shared_ptr<Session>>_sessions;
    
};

#endif 


