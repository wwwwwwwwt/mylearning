/*
 * @Author: zzzzztw
 * @Date: 2023-06-05 19:49:25
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-16 13:44:07
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/CSession.h
 */
#pragma once
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <queue>
#include <mutex>
#include <memory>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include "msgnode.h"
#include "const.h"
using namespace std;
using boost::asio::ip::tcp;
class CServer;
class LogicSystem;
class CSession:public std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::io_context& io_context, CServer* server);
	~CSession() {
		std::cout << "Session destruct" << endl;
	}
	tcp::socket& GetSocket();
	std::string& GetUuid();
	void Start();
	void Send(char* msg, short max_length, short msgid);
	void Send(std::string msg, short msg_id);
	void Close();
	std::shared_ptr<CSession>SharedSelf(){
		return shared_from_this();
	}
private:
	void HandleRead(const boost::system::error_code& error, size_t  bytes_transferred, shared_ptr<CSession> _self_shared);
	void HandleWrite(const boost::system::error_code& error, shared_ptr<CSession> _self_shared);
	tcp::socket _socket;
	bool _b_close;
	std::string _uuid;
	char _data[MAX_LENGTH];
	CServer* _server;
	std::queue<shared_ptr<SendNode> > _send_que;
	std::mutex _send_lock;
	boost::asio::strand<boost::asio::io_context::executor_type>_strand;
	// 处理粘包逻辑，保存头部和消息体，还有flag表示头部解析是否完成

	std::shared_ptr<RecvNode>_recv_msg_node;
	bool _b_head_parse;
	std::shared_ptr<MsgNode>_recv_head_node;
};

class LogicNode{
	friend class LogicSystem;
public:
	LogicNode(shared_ptr<CSession>, shared_ptr<RecvNode>);

private:
	shared_ptr<CSession>_session;
	shared_ptr<RecvNode>_recvnode;
};