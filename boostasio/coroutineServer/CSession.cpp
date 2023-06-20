/*
 * @Author: zzzzztw
 * @Date: 2023-06-20 15:01:19
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-20 15:45:07
 * @FilePath: /myLearning/boostasio/coroutineServer/CSession.cpp
 */
#include "CSession.h"
#include "CServer.h"
CSession::CSession(boost::asio::io_context&ioc, CServer* server):_io_context(ioc),_server(server)
,_socket(ioc)
{

}

boost::asio::ip::tcp::socket& CSession::GetSocket(){
    return _socket;
}