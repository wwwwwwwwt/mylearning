#include "Session.h"
#include <iostream>

using namespace std;

void Session::Start(){
    memset(_data, 0, max_length);
    // 绑定一个读事件，填进底层的epoll时间表，并将数据自动的读到buffer中，proactor的特性，异步处理
    // 注意这里不可以使用shared_ptr<Session>(this)这种方法，会造成多个智能指针一起管理同一块内存的错误，引用计数不一致
    _socket.async_read_some(boost::asio::buffer(_data, max_length), std::bind(&Session::handler_read, this,
        std::placeholders::_1, std::placeholders::_2, shared_from_this())); 
}

void Session::handler_read(const boost::system::error_code& err, std::size_t bytes_transferred, std::shared_ptr<Session>_self_ptr){

    if(!err){
        cout<<"server receice data is "<<_data<<endl;
        boost::asio::async_write(_socket, boost::asio::buffer(_data, bytes_transferred),
            std::bind(&Session::handler_write, this, std::placeholders::_1,  _self_ptr));
        

    }
    else{
        cout<<"read err! "<<err.value()<<endl;
      //  delete this; // 生产环境中不会这么做
        _server->ClearSession(GetUUid());
    }
}

void Session::handler_write(const boost::system::error_code& err,std::shared_ptr<Session>_self_ptr){
    if(!err){
        memset(_data, 0, sizeof _data);
        cout<<"server receive data is "<< _data<<endl;
        _socket.async_read_some(boost::asio::buffer(_data, sizeof _data), std::bind(&Session::handler_read, this, std::placeholders::_1,
        std::placeholders::_2, _self_ptr));
    }else{
        cout<<"write err! "<<err.value()<<endl;
       // delete this; // 生产环境中不会这么做
       _server->ClearSession(GetUUid());
    }
}

Server::Server(boost::asio::io_context& ioc, short port):_ioc(ioc), _acceptor(ioc, tcp::endpoint(tcp::v4(), port))
{
    std::cout<<"Server start success on port"<<port<<std::endl;
    start_accept();
}

void Server::start_accept(){
    // session类似于服务人员，acceptor类似大堂经理，大堂经理把任务给服务人员，叫他去处理。
    // Session* new_session = new Session(_ioc);
    std::shared_ptr<Session>new_session = std::make_shared<Session>(_ioc, this);
    _acceptor.async_accept(new_session->Socket(), std::bind(&Server::handle_accept, this, new_session, std::placeholders::_1));//进来一个连接，服务器使用newsession保存这个连接。
}

void Server::handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& ec){
    if(!ec){
        new_session->Start();
        _sessions.insert({new_session->GetUUid(), new_session});
    }
    else{
        
    }
    // 处理完了，在创建一个startaccept 去处理新连接
    start_accept();
}

void Server::ClearSession(std::string uuid){
    _sessions.erase(uuid);
}

