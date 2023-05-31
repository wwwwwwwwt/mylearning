/*
 * @Author: zzzzztw
 * @Date: 2023-05-29 14:03:40
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-30 20:17:41
 * @FilePath: /myLearning/boostasio/endpoints..cpp
 */

#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "endpoint.h"
#include <memory>
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


//asio网络库中使用buffer麻烦的方式
void use_const_buffer(){
    std::string buf = "Hello world";
    asio::const_buffer asio_buf(buf.c_str(), buf.size());
    std::vector<asio::const_buffer>buffers_seq;
    buffers_seq.push_back(asio_buf); // 最终的buffer_seq就是可以传递给send的类型， 可以直接使用buffer转换为send需要的类型。
}

// 简单的方式
void use_const_str(){
    asio::const_buffers_1 output_buf = asio::buffer("hello, world");
}
// 字符串数组
void use_buffer_array(){
    const size_t BUF_SIZE_BYTES = 20;
    std::unique_ptr<char[]>buf(new char[BUF_SIZE_BYTES]);
    auto input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
}


//使用streanbuf，将输入输出流和streambuf绑定，可以实现流式输入和输出
void use_stream_buffer(){

    asio::streambuf buf;

    std::ostream output(&buf);

    output<<"Message\nMessage2";

    std::istream input(&buf);

    std::string message1;

    std::getline(input, message1);

}

//将buf中的数据发送给sock， 一点一点发，每次发了多少会返回给用户端
void write_to_socket(asio::ip::tcp::socket& sock){

    std::string buf = "hello world";
    std::size_t total_bytes_written = 0;

    // 循环发送
    //wirte_som 返回每次写入的字节数
    while(total_bytes_written != buf.size()){
        total_bytes_written += sock.write_some(asio::buffer(buf.c_str() + total_bytes_written, buf.size() - total_bytes_written));
    }
}



//模拟同步客户端，先创建对端地址，创建上下文，创建socket，使用socket连接到对端
int send_data_by_write_some(){
    std::string raw_ip_address = "192.168.3.11";
    unsigned short port_num = 3333;

    try{
        asio::ip::tcp::endpoint ep(asio::ip::address_v4::from_string(raw_ip_address), port_num);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
        //write_to_socket(sock); // 一点一点发送
        /*
            一次性全发送完，发送完再通知
            阻塞到全发送完，返回发送的字节数，返回值小于等于0就说明出现了错误
        */
        std::string buf = "hello world";
        int sendlength = sock.send(asio::buffer(asio::buffer(buf.c_str(), buf.size())));
        if(sendlength <=0)return 0;

    }catch(boost::system::system_error& e){
        std::cout<<"Error failed! error code "<<e.code().value()<<" message is "<<e.what()<<std::endl;
        return e.code().value();
    }
}

//读到数据
std::string read_from_socket(asio::ip::tcp::socket& sock){
    const unsigned short MESSAGE_SIZE = 7;
    char buf[MESSAGE_SIZE];
    std::size_t total_bytes_read = 0;
    while(total_bytes_read != MESSAGE_SIZE){
        total_bytes_read += sock.read_some(asio::buffer(buf + total_bytes_read, MESSAGE_SIZE - total_bytes_read));
    }

    return std::string(buf, total_bytes_read);
}

//模仿客户端读数据

int read_data_by_read_some(){
    
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try{
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
        // read_from_socket(sock);一点一点读
        const size_t BUF_SIZE_BYTES = 20;
        std::unique_ptr<char[]>buf(new char[BUF_SIZE_BYTES]);
        auto input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
        int length = sock.receive(input_buf);
        if(length <= 0)return 0;


    }catch(boost::system::system_error& e){
        std::cout<<"Error failed! error code "<<e.code().value()<<" message is "<<e.what()<<std::endl;
        return e.code().value();
    }

}