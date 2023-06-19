/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 22:24:45
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-18 22:41:58
 * @FilePath: /cpptest/boostasio/coroutineServer/CServer.h
 */
#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <map>
#include "CSession.h"
#include "IoServicePool.h"
class CServer{
public:
    CServer(boost::asio::io_context& io_context, short port);
    ~CServer();
    void ClearSession(std::string);
private:
    void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code& err);
    void StartAccept();
    boost::asio::io_context& io_context;
    short _port;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<CSession>>_sessions;
};