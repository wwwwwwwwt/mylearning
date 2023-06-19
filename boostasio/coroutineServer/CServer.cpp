/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 22:36:38
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-18 22:43:18
 * @FilePath: /cpptest/boostasio/coroutineServer/CServer.cpp
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
    
}