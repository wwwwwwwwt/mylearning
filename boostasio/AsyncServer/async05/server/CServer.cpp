/*
 * @Author: zzzzztw
 * @Date: 2023-06-05 19:48:45
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-16 15:16:57
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/CServer.cpp
 */
#include "CServer.h"
#include <iostream>
CServer::CServer(boost::asio::io_context& io_context, short port):_io_context(io_context), _port(port),
_acceptor(io_context, tcp::endpoint(tcp::v4(),port)), work_guard_(boost::asio::make_work_guard(io_context))
{
	cout << "Server start success, listen on port : " << _port << endl;
	StartAccept();
}
CServer::~CServer() {
	cout << "Server destruct listen on port : " << _port << endl;
}
void CServer::HandleAccept(shared_ptr<CSession> new_session, const boost::system::error_code& error){
	if (!error) {
		new_session->Start();
		lock_guard<mutex> lock(_mutex);
		_sessions.insert(make_pair(new_session->GetUuid(), new_session));
	}
	else {
		cout << "session accept failed, error is " << error.value() << endl;
	}

	StartAccept();
}

void CServer::StartAccept() {

	// 增加io_context池逻辑
	auto& io_context = AsioIOServivePool::GetInstance()->GetIOServive();

	shared_ptr<CSession> new_session = make_shared<CSession>(io_context, this);
	_acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, placeholders::_1));
}

void CServer::ClearSession(std::string uuid) {
	lock_guard<mutex> lock(_mutex);
	_sessions.erase(uuid);
}