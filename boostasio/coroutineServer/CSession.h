/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 22:33:23
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-20 15:30:19
 * @FilePath: /myLearning/boostasio/coroutineServer/CSession.h
 */
#pragma once
#include <memory>
#include <boost/asio.hpp>
class CServer;
class CSession:public std::enable_shared_from_this<CSession>
{
public:
    CSession(boost::asio::io_context& io_context, CServer* server);
    ~CSession();
    boost::asio::ip::tcp::socket& GetSocket();
    std::string Getuuid()const;
    void Start();
private:
    boost::asio::io_context& _io_context;
    CServer* _server;
    boost::asio::ip::tcp::socket _socket;
};