#include <boost/asio.hpp>
#include <iostream>
#include <string>
using namespace boost;
using namespace std;
const int MAX_LENGTH = 1024;

int main(){
    try{
        // 创建上下文服务
        asio::io_context ioc;
        // 构造endpoint
        std::string addr = "127.0.0.1";
        unsigned short port_num = 10086; 
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(addr), port_num);
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        boost:system::error_code err = asio::error::host_not_found;
        sock.connect(ep, err);
        if(err){
            cout<<"connect failed, code is "<<err.value()<<"error msg is "<<err.message()<<endl;
            return 0;
        }  

        //客户端向服务端发送信息
        cout<<"enter message: ";
        char request[MAX_LENGTH];
        std::cin.getline(request, MAX_LENGTH);
        size_t request_len = strlen(request);
        boost::asio::write(sock, boost::asio::buffer(static_cast<void*>(request), request_len));

        // 接收服务端发来的消息。
        char reply[MAX_LENGTH];
        size_t reply_len = boost::asio::read(sock, asio::buffer(static_cast<void*>(reply), request_len));
        std::cout<<"reply is ";
        std::cout.write(reply, reply_len);
        cout<<"\n";

    }catch(boost::system::system_error& e){

        std::cerr<<"exception:" <<e.what()<<endl;

    }

    return 0;
}