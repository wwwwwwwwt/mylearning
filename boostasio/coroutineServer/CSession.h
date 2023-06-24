/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 22:33:23
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-24 16:16:50
 * @FilePath: /myLearning/boostasio/coroutineServer/CSession.h
 */
#pragma once
#include <memory>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <queue>
#include "const.h"
#include <mutex>
#include "msgnode.h"

class CServer;
class CSession:public std::enable_shared_from_this<CSession>
{
public:
    CSession(boost::asio::io_context& io_context, CServer* server);
    ~CSession();
    boost::asio::ip::tcp::socket& GetSocket();
    std::string Getuuid();
    void Start();
private:
    boost::asio::io_context& _io_context;
    CServer* _server;
    boost::asio::ip::tcp::socket _socket;
    std::string _uuid;
};