/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 22:24:45
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-24 15:35:50
 * @FilePath: /myLearning/boostasio/coroutineServer/CServer.h
 */
#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <map>
#include <mutex>
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
    std::mutex mtx_;
};