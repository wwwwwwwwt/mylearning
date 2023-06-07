/*
 * @Author: zzzzztw
 * @Date: 2023-06-05 19:49:25
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-07 19:36:04
 * @FilePath: /myLearning/boostasio/AsyncServer/async03/server/CSession.h
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
using namespace std;
#define MAX_LENGTH  1024 * 2
#define HEAD_LENGTH 2
using boost::asio::ip::tcp;
class CServer;

class MsgNode
{
	friend class CSession;
public:
	MsgNode(const char * msg, short max_len) : _total_len(max_len + HEAD_LENGTH), _cur_len(0){
		_data = new char[_total_len + 1];
		//尽量使用long 避免精度丢失
		unsigned short max_len_host = boost::asio::detail::socket_ops::host_to_network_short(max_len); 
		memcpy(_data, &max_len_host, HEAD_LENGTH); // 把包的实际长度转换为网络字节序，并拷贝到头部
		memcpy(_data + HEAD_LENGTH, msg, max_len);
		_data[_total_len] = '\0';
	}

	MsgNode(short max_len):_total_len(max_len), _cur_len(0){
		_data = new char[_total_len + 1];
	}
	~MsgNode() {
		delete[] _data;
	}

	void Clear(){
		memset(_data, 0, sizeof _data);
		_cur_len = 0;
	}

private:
	int _cur_len;
	int _total_len;
	char* _data;
};
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
	void Send(char* msg,  int max_length);
	void Send(std::string msg);
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
	std::queue<shared_ptr<MsgNode> > _send_que;
	std::mutex _send_lock;

	// 处理粘包逻辑，保存头部和消息体，还有flag表示头部解析是否完成

	std::shared_ptr<MsgNode>_recv_msg_node;
	bool _b_head_parse;
	std::shared_ptr<MsgNode>_recv_head_node;
};