/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 22:33:23
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-18 22:47:45
 * @FilePath: /cpptest/boostasio/coroutineServer/CSession.h
 */
#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "CServer.h"
class CSession:public std::enable_shared_from_this<CSession>
{
public:
    CSession(boost::asio::io_context& io_context, CServer* server);
    ~CSession();
private:
    boost::asio::io_context& _io_context;
    CServer* _server;
};