/*
 * @Author: zzzzztw
 * @Date: 2023-05-31 12:43:19
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-07 11:10:04
 * @FilePath: /myLearning/boostasio/sync/syncClient/syncClient.cpp
 */
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
using namespace std;
using namespace boost::asio::ip;
const int MAX_LENGTH = 1024*2;
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

		thread send_thread([&sock] {
			for (;;) {
				this_thread::sleep_for(std::chrono::milliseconds(2));
				const char* request = "hello world!";
				size_t request_length = strlen(request);
				char send_data[MAX_LENGTH] = { 0 };
				short length = boost::asio::detail::socket_ops::host_to_network_short(request_length);
				memcpy(send_data, &length, 2);
				memcpy(send_data + 2, request, request_length);
				boost::asio::write(sock, boost::asio::buffer(send_data, request_length + 2));
			}
			});

		thread recv_thread([&sock] {
			for (;;) {
				this_thread::sleep_for(std::chrono::milliseconds(2));
				cout << "begin to receive..." << endl;
				char reply_head[HEAD_LENGTH];
				size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply_head, HEAD_LENGTH));
				short msglen = 0;
				memcpy(&msglen, reply_head, HEAD_LENGTH);
				msglen = boost::asio::detail::socket_ops::network_to_host_short(msglen);
				char msg[MAX_LENGTH] = { 0 };
				size_t  msg_length = boost::asio::read(sock, boost::asio::buffer(msg, msglen));

				std::cout << "Reply is: ";
				std::cout.write(msg, msglen) << endl;
				std::cout << "Reply len is " << msglen;
				std::cout << "\n";
			}
			});

		send_thread.join();
		recv_thread.join();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << endl;
	}
	return 0;
}