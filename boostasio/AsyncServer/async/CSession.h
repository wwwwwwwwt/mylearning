/*
 * @Author: zzzzztw
 * @Date: 2023-06-05 19:49:25
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-05 19:52:52
 * @FilePath: /myLearning/boostasio/AsyncServer/async/CSession.h
 */
#pragma once
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <queue>
#include <mutex>
#include <memory>
#include <iostream>
using namespace std;
#define MAX_LENGTH  1024
using boost::asio::ip::tcp;
class CServer;

class MsgNode
{
	friend class CSession;
public:
	MsgNode(char * msg, int max_len) {
		_data = new char[max_len];
		memcpy(_data, msg, max_len);
	}

	~MsgNode() {
		delete[] _data;
	}

private:
	int _cur_len;
	int _max_len;
	char* _data;
};
class CSession:public std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::io_context& io_context, CServer* server);
	~CSession() {
		std::cout << "Ssession destruct" << endl;
	}
	tcp::socket& GetSocket();
	std::string& GetUuid();
	void Start();
	void Send(char* msg,  int max_length);
private:
	void HandleRead(const boost::system::error_code& error, size_t  bytes_transferred, shared_ptr<CSession> _self_shared);
	void HandleWrite(const boost::system::error_code& error, shared_ptr<CSession> _self_shared);
	tcp::socket _socket;
	std::string _uuid;
	char _data[MAX_LENGTH];
	CServer* _server;
	std::queue<shared_ptr<MsgNode> > _send_que;
	std::mutex _send_lock;
};