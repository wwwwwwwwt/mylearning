/*
 * @Author: zzzzztw
 * @Date: 2023-06-08 13:06:37
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-08 13:06:41
 * @FilePath: /myLearning/boostasio/AsyncServer/async04/client/client.cpp
 */
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
using namespace std;
using namespace boost::asio::ip;
const int MAX_LENGTH = 1024 * 2;
const int HEAD_LENGTH = 2;
int main()
{
	try {
		//创建上下文服务
		boost::asio::io_context   ioc;
		//构造endpoint
		tcp::endpoint  remote_ep(address::from_string("127.0.0.1"), 10086);
		tcp::socket  sock(ioc);
		boost::system::error_code   error = boost::asio::error::host_not_found; ;
		sock.connect(remote_ep, error);
		if (error) {
			cout << "connect failed, code is " << error.value() << " error msg is " << error.message();
			return 0;
		}

		Json::Value root;
		root["id"] = 1001;
		root["data"] = "hello world";
		std::string request = root.toStyledString();
		size_t request_length = request.length();
		char send_data[MAX_LENGTH] = { 0 };
		//转为网络字节序
		int request_host_length = boost::asio::detail::socket_ops::host_to_network_short(request_length);
		memcpy(send_data, &request_host_length, 2);
		memcpy(send_data + 2, request.c_str(), request_length);
		boost::asio::write(sock, boost::asio::buffer(send_data, request_length + 2));
		cout << "begin to receive..." << endl;
		char reply_head[HEAD_LENGTH];
		size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply_head, HEAD_LENGTH));
		short msglen = 0;
		memcpy(&msglen, reply_head, HEAD_LENGTH);
		//转为本地字节序
		msglen = boost::asio::detail::socket_ops::network_to_host_short(msglen);
		char msg[MAX_LENGTH] = { 0 };
		size_t  msg_length = boost::asio::read(sock, boost::asio::buffer(msg, msglen));
		Json::Reader reader;
		reader.parse(std::string(msg,msg_length),root);
		std::cout << "msg id is " << root["id"] << " msg is " << root["data"] << endl;
		getchar();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << endl;
	}
	return 0;
}
