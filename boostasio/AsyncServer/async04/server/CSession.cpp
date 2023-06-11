/*
 * @Author: zzzzztw
 * @Date: 2023-06-05 19:49:32
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-08 19:00:27
 * @FilePath: /myLearning/boostasio/AsyncServer/async04/server/CSession.cpp
 */
#include "CSession.h"
#include "CServer.h"
#include <iostream>
CSession::CSession(boost::asio::io_context& io_context, CServer* server):
	_socket(io_context), _server(server){
	boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	_uuid = boost::uuids::to_string(a_uuid);
	_recv_head_node = std::make_shared<MsgNode>(HEAD_LENGTH);
}

tcp::socket& CSession::GetSocket() {
	return _socket;
}

std::string& CSession::GetUuid() {
	return _uuid;
}

void CSession::Start(){
	memset(_data, 0, MAX_LENGTH);
	_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, 
		std::placeholders::_1, std::placeholders::_2, shared_from_this()));
}

void CSession::Send(char* msg, int max_length) {
	
	std::lock_guard<std::mutex> lock(_send_lock);
	bool pending = false;
	if (_send_que.size() > 0) {
		pending = true;
	}
	_send_que.push(make_shared<MsgNode>(msg, max_length));
	if (pending) {
		return;
	}
	auto& msgnode = _send_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len), 
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));
}

void CSession::Send(std::string msg) {
	std::lock_guard<std::mutex> lock(_send_lock);
	int send_que_size = _send_que.size();

	_send_que.push(make_shared<MsgNode>(msg.c_str(), msg.length()));
	if (send_que_size > 0) {
		return;
	}
	auto& msgnode = _send_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, SharedSelf()));
}

void CSession::Close(){
	_socket.close();
	_b_close = true;
}

void CSession::HandleWrite(const boost::system::error_code& error, shared_ptr<CSession> _self_shared) {
	if (!error) {
		std::lock_guard<std::mutex> lock(_send_lock);
		_send_que.pop();
		if (!_send_que.empty()) {
			auto &msgnode = _send_que.front();
			boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
				std::bind(&CSession::HandleWrite, this, std::placeholders::_1, _self_shared));
		}
	}
	else {
		std::cout << "handle write failed, error is " << error.value() << endl;
		Close();
		_server->ClearSession(_uuid);
	}
}

void CSession::HandleRead(const boost::system::error_code& error, size_t  bytes_transferred, shared_ptr<CSession> _self_shared){
/*	if (!error) {

		cout << "read data is " << _data << endl;
		//
		Send(_data, bytes_transferred);
		memset(_data, 0, MAX_LENGTH);
		_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, 
			std::placeholders::_1, std::placeholders::_2, _self_shared));
	}
	else {
		std::cout << "handle read failed, error is " << error.value() << endl;


		_server->ClearSession(_uuid);
	}*/


	// 处理粘包逻辑
	if(!error){
		int copy_len = 0;
		while(bytes_transferred > 0){
			if(!_b_head_parse){
				//当前收到的数据 加上本来的数据还没到头部长度，就注册回调函数，继续接收
				if(bytes_transferred + _recv_head_node->_cur_len < HEAD_LENGTH){
					memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, bytes_transferred);
					_recv_head_node->_cur_len += bytes_transferred;
					memset(_data, 0, sizeof _data);
					_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, 
					std :: placeholders::_1, std::placeholders::_2, _self_shared));
					return;
				}

				//当前收到的数据加上已有的数据比头部多了,填满head再把剩下的向body中填
				int head_remain = HEAD_LENGTH - _recv_head_node->_cur_len;
				memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, head_remain);
				copy_len += head_remain;
				bytes_transferred -= head_remain;

				//从头部获取这个消息中实际的数据长度
				short data_len = 0;
				//接收网络字节序数据
				memcpy(&data_len, _recv_head_node->_data, HEAD_LENGTH);
				//网络字节序转换为主机字节序
				data_len = boost::asio::detail::socket_ops::network_to_host_short(data_len);

				cout<<"data_len is "<<data_len<<endl;

				//如果头部长度非法，直接关闭连接
				if(data_len > MAX_LENGTH){
					std::cout << "invalid data length is " << data_len << endl;
					_server->ClearSession(_uuid);
					return;		
				}

				//拿到头部的实际消息长度，开始处理消息体
				_recv_msg_node = std::make_shared<MsgNode>(data_len);
				//如果当前剩下的消息，还不够实际消息的全部长度，就全部填入消息体，然后接着读
				if(bytes_transferred < data_len){
					memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
					_recv_msg_node->_cur_len += bytes_transferred;
					memset(_data, 0, MAX_LENGTH);
					_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, 
					std::placeholders::_1, std::placeholders::_2, _self_shared));
					//走到这里逻辑是头部处理完成了，消息体还没满
					_b_head_parse = true;
					return;
				}

				// 否则，当前剩下的消息数据，满足这个包内容的实际长度，就将实际内容大小的消息长度，填充进消息体，然后就解析完这个包。重新将解析头标志位归false.
				// 如果剩下数据长度还有多余的，就continue，否则绑定回调函数，return
				memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, data_len);
				_recv_msg_node->_cur_len += data_len;
				copy_len += data_len;
				bytes_transferred -= data_len;
				_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
				//cout<<"receive data is "<<_recv_msg_node->_data<<endl;
				Json::Reader reader;
				Json::Value root;
				reader.parse(std::string(_recv_msg_node->_data, _recv_msg_node->_total_len), root);
				std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is " << root["data"].asString() << endl;
				root["data"] = "server has received msg, msg data is " + root["data"].asString();
				std::string return_str = root.toStyledString();
				// 拿到数据包后的逻辑处理，此处暂时发送回客户端
			//	Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
				Send(return_str);
				// 继续轮询剩余的未处理数据
				_b_head_parse = false;
				_recv_head_node->Clear();
				if(bytes_transferred <= 0){
					memset(_data, 0, MAX_LENGTH);
					_socket.async_read_some(boost::asio::buffer(static_cast<void *>(_data), MAX_LENGTH), std::bind(&CSession::HandleRead, this,
					std::placeholders::_1, std::placeholders::_2, _self_shared));
					return;
				}
				// 还有剩余的，继续轮询处理下一个包的数据
				continue;
			}

			//如果这时候状态是已经处理完头部了，那么剩下的数据就要向内容里填
			int remain_msg = _recv_msg_node->_total_len - _recv_msg_node->_cur_len;
			//如果当前读到的，小于实际总的，就全填进去，注册读回调，接着读
			if(bytes_transferred < remain_msg){
				memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
				_recv_msg_node->_cur_len += bytes_transferred;
				memset(_data, 0, MAX_LENGTH);
				_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, 
				std::placeholders::_1, std::placeholders::_2, _self_shared));
				return;
			}
			memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, remain_msg);
			_recv_msg_node->_cur_len += remain_msg;
			bytes_transferred -= remain_msg;
			copy_len += remain_msg;
			_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
			//cout<<"receive data is "<<_recv_msg_node->_data<<endl;

			Json::Reader reader;
			Json::Value root;
			reader.parse(std::string(_recv_msg_node->_data, _recv_msg_node->_total_len), root);
			std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is " << root["data"].asString() << endl;
			root["data"] = "server has received msg, msg data is " + root["data"].asString();
			std::string return_str = root.toStyledString();
			Send(return_str);
			// 否则，读到这个包所有数据，拿到数据包后的逻辑处理，此处暂时发送回客户端
			//Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
			_recv_head_node->Clear();
			if(bytes_transferred <= 0){
				memset(_data, 0, MAX_LENGTH);
				_socket.async_read_some(boost::asio::buffer(static_cast<void *>(_data), MAX_LENGTH), std::bind(&CSession::HandleRead, this,
				std::placeholders::_1, std::placeholders::_2, _self_shared));
				return;
			}
			continue;
		}
	}
	else{
		// 报错
		std::cout << "handle read failed, error is " << error.value() << endl;
        Close();
        _server->ClearSession(_uuid);
	}
	
}