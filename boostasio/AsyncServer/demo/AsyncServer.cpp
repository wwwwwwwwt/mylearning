#include <iostream>
#include "Session.h"
#include <boost/asio.hpp>
#include <exception>

int main(){

    try{
        boost::asio::io_context ioc;
        using namespace std;
        Server s(ioc, 10086);
        // io_context io.run(), 开始轮询
        //启动异步事件循环。这个事件循环会在需要处理的异步操作完成时自动触发相关的回调函数，以及在没有待处理事件时休眠以节省系统资源。
        ioc.run();

    }catch(std::exception& e){
        std::cout<<"err ! err is "<<e.what()<<std::endl;
    }

}