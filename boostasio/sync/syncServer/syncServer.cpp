#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <set>
using namespace std;
using namespace boost;
const int MAX_LENGTH = 1024;

std::set<std::shared_ptr<std::thread>>thread_set;
//处理一个会话
void session(std::shared_ptr<asio::ip::tcp::socket> sock){
    try{ 
        for(;;){
            char data[MAX_LENGTH];
            memset(data, '\0', sizeof data);
            boost::system::error_code err;
            //size_t length = asio::read(sock, asio::buffer(static_cast<void*>(data), MAX_LENGTH), err);
            //从客户端中读出消息进buffer
            size_t length = sock->read_some(asio::buffer(static_cast<void*>(data), MAX_LENGTH), err);

            if(err == boost::asio::error::eof){
                // err为eof为对端进行了关闭。
                std::cout<<"connet closed by peer"<< endl;
                break;
            }
            else if(err){
                //否则是严重错误，抛出
                throw system::error_code(err);
            }else{
                //到这里就正常接受数据的逻辑，打印下客户端连接进来的信息和数据
                cout<<"receive from "<<sock->remote_endpoint().address().to_string()<<endl;
                cout<<data<<endl;
                //把收到的数据回传给对方, 
                boost::asio::write(*sock,asio::buffer(static_cast<void*>(data), length));

            }
        }

    }catch(boost::system::system_error& e){
        std::cerr<<"exception in "<<e.what()<<"\n";
    }
}

// 

void server(boost::asio::io_context& ios, unsigned short port){
    asio::ip::tcp::acceptor a(ios,asio::ip::tcp::endpoint(asio::ip::address_v4::any(), port));
    for(;;){
        std::shared_ptr<asio::ip::tcp::socket>sock = make_shared<asio::ip::tcp::socket>(ios);
        a.accept(*sock);
        auto t = std::make_shared<std::thread>(session, sock); //这次for循环后，这个智能指针会被析构，所以为了延长其生命周期，我们将其放在一个线程集合中
        thread_set.insert(t);
    }
}

int main(){

    try{
        boost::asio::io_context ioc;
        server(ioc, 10086);  
        for(auto &t : thread_set){
            t->join(); //主线程等待所有线程执行完再退出
        }      
    }catch(std::exception& e){

    }


}