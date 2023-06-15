/*
 * @Author: zzzzztw
 * @Date: 2023-06-08 19:00:00
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-15 15:53:32
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/AsyncServer.cpp
 */
#include <iostream>
#include "CSession.h"
#include "CServer.h"
#include <boost/asio.hpp>
#include <exception>
#include <signal.h>
#include <thread>
#include <condition_variable>
#include "AsioIOServivePool.h"
using namespace std;
int main(){

    try{
        auto pool = AsioIOServivePool::GetInstance();
        boost::asio::io_context ioc;
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&](auto , auto){
            ioc.stop();
            pool->Stop();
        });
        CServer s(ioc, 10086);
       // io_context io.run(), 开始轮询
       // 启动异步事件循环。这个事件循环会在需要处理的异步操作完成时自动触发相关的回调函数，以及在没有待处理事件时休眠以节省系统资源。
        ioc.run();

    }catch(std::exception& e){
        std::cout<<"err ! err is "<<e.what()<<std::endl;
    }

}

// std::condition_variable cv_;
// std::mutex mtx_;
// volatile bool stop_ = false;
// void signal_handler(int sig){
//     if(sig == SIGINT || sig == SIGTERM){
//         unique_lock<mutex>lokcer(mtx_);
//         stop_ = true;
//         cv_.notify_all();
//     }
// }

// int main(){
//     try{
//         boost::asio::io_context ioc;
//         std::thread t([&](){
//             CServer s(ioc, 10086);
//             ioc.run();
//         });
//         signal(SIGINT,signal_handler);
//         signal(SIGTERM, signal_handler);
//         unique_lock<mutex>locker(mtx_);
//         cv_.wait(locker, [&]{
//             return stop_ == true;
//         });
//         ioc.stop();
//         t.join();

//     }catch(std::exception& e){
//         std::cout<<"err! err is "<<e.what()<<std::endl;
//     }
// }