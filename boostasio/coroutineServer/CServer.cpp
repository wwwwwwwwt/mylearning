/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 22:36:38
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-20 16:01:34
 * @FilePath: /myLearning/boostasio/coroutineServer/CServer.cpp
 */
#include "CServer.h"

CServer::CServer(boost::asio::io_context&ioc, short port):io_context(ioc), _port(port)
,_acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port))
{
    StartAccept();
}

CServer::~CServer(){

}

void CServer::StartAccept(){
    auto& io_context = AsioIoServivePool::GetInstance().GetIOService();
    std::shared_ptr<CSession>new_session = std::make_shared<CSession>(io_context, this);
    _acceptor.async_wait(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
}

void CServer::HandleAccept(std::shared_ptr<CSession>new_session, const boost::system::error_code& err){
    if(!err){
        new_session->Start();
        std::lock_guard<std::mutex>locker(mtx_);
        _session.insert({new_session->Getuuid(), new_session});
    }else{
       std::cout<<"session accept failed, error is"<<err.value()<<std::endl; 
    }
}