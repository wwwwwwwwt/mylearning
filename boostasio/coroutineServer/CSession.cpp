/*
 * @Author: zzzzztw
 * @Date: 2023-06-20 15:01:19
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-24 16:32:06
 * @FilePath: /myLearning/boostasio/coroutineServer/CSession.cpp
 */
#include "CSession.h"
#include "CServer.h"
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::strand;
namespace this_core = boost::asio::this_coro;

CSession::CSession(boost::asio::io_context&ioc, CServer* server):_io_context(ioc),_server(server)
,_socket(ioc)
{
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
    _uuid = boost::uuids::to_string(a_uuid);
}

CSession::~CSession(){

}

boost::asio::ip::tcp::socket& CSession::GetSocket(){
    return _socket;
}

std::string CSession::Getuuid(){
    return _uuid;
}
void CSession::Start(){
    auto shared_this = shared_from_this();
    boost::asio::co_spawn(_io_context, [=]()->boost::asio::awaitable <void>{
        try{

        }catch(std::exception& e){

        }
    }, detached);
}