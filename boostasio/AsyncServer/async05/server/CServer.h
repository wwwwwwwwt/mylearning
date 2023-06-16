/*
 * @Author: zzzzztw
 * @Date: 2023-06-05 19:48:57
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-16 14:59:44
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/CServer.h
 */
#pragma once
#include <boost/asio.hpp>
#include "AsioIOServivePool.h"
#include "CSession.h"
#include <memory.h>
#include <map>
using namespace std;
using boost::asio::ip::tcp;
class CServer
{
public:
	CServer(boost::asio::io_context& io_context, short port);
	~CServer();
	void ClearSession(std::string);
private:
	void HandleAccept(shared_ptr<CSession>, const boost::system::error_code & error);
	void StartAccept();
	boost::asio::io_context &_io_context;
	short _port;
	tcp::acceptor _acceptor;
	std::map<std::string, shared_ptr<CSession>> _sessions;
	std::mutex _mutex;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
};
