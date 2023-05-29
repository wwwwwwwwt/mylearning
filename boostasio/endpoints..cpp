/*
 * @Author: zzzzztw
 * @Date: 2023-05-29 14:03:40
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-29 15:51:09
 * @FilePath: /myLearning/boostasio/endpoints..cpp
 */

#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "endpoint.h"
using namespace boost;
using namespace std;


//如果是客户端，我们可以通过对端的ip和端口构造一个endpoint，用这个endpoint和其通信。
int client_end_point(){

    std::string row_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;


    boost::system::error_code ec;
    asio::ip::address ip_address = asio::ip ::address::from_string(row_ip_address, ec); // 解析我们想连接的ip

    if(ec.value()!= 0){
        cout<<"Failed to parse Ip address. error code is" << ec.value()<<",Message is " << ec.message()<<endl;
        return ec.value();
    }

    asio::ip::tcp::endpoint ep(ip_address, port_num); // 生成我们要使用的IP地址 ip + port

    return 0;
}

//服务端，则只需根据本地地址绑定就可以生成endpoint
int server_end_point(){
    unsigned short port_num = 3333;
    asio::ip::address ip_adress = asio::ip::address_v4::any();
    asio::ip::tcp::endpoint ep(ip_adress, port_num);
    return 0;
}


//创建socket分为4步，创建上下文iocontext，选择协议，生成socket，打开socket。
int create_tcp_socket(){
    asio::io_context ioc;
    asio::ip::tcp protocol = asio::ip::tcp::v4();
    asio::ip::tcp::socket sock(ioc);
    boost::system::error_code ec;
    sock.open(protocol, ec);
    if(ec.value() != 0){
        cout<<"Failed err!"<<endl;
        return ec.value();
    }
    return 0;
}

//上述socket只是通信的socket，如果是服务端，我们还需要生成一个acceptor的socket，用来接收新的连接。
int create_acceptor_socket(){
    asio::io_context ios;
    // asio::ip::tcp::acceptor acceptor(ios);
    // asio::ip::tcp protocol = asio::ip::tcp::v4();
    // boost::system::error_code ec;
    // acceptor.open(protocol, ec);
    // if(ec.value() != 0){
    //     cout<<"Failed"<<endl;
    //     return ec.value();
    // }
    //新版简便方式 创建 + 绑定端口
    asio::ip::tcp::acceptor acceptor(ios, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 3333));

    return 0;
}
//老版本bind用法 对于acceptor类型的socket，服务器要将其绑定到指定的端点,所有连接这个端点的连接都可以被接收到。
int bind_acceptor_socket(){
    // 创建端口号
    unsigned short port_num = 3333;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
    asio::io_context ios;
    asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
    boost::system::error_code ec;
    acceptor.bind(ep, ec);
    return 0;
}


//作为客户端可以连接服务器指定的端点进行连接

int connect_to_end(){

    // 1. 创建连接的端口号和ip地址
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;

    try{
        //2. 创建连接目标端点
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        
        //3. 创建sock并绑定bind端口
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        
        //4. 与对端建立连接
        sock.connect(ep);
    }catch(boost::system::system_error& e){
        std::cout<<"error code: "<<e.code()<<" message: "<<e.what();
        return e.code().value(); 
    }

    return 0;

}

int accept_new_connection(){
    const int BLACKLOG_SIZE = 30;
    unsigned short port_num = 3333;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
    asio::io_context ios;

    try{
        //创建一个连接器
        asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
        // 绑定我们需要对外开放的服务器终端地址
        acceptor.bind(ep);
        //监听这个地址
        acceptor.listen(BLACKLOG_SIZE);

        //建立连接返回一个新的socket，用于通信。
        asio::ip::tcp::socket sock(ios);
        acceptor.accept(sock);

    }catch(boost::system::system_error& e){
        std::cout<<"error code: "<<e.code()<<" message: "<<e.what();
        return e.code().value(); 
    }
}