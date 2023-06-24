/*
 * @Author: zzzzztw
 * @Date: 2023-06-24 15:37:31
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-24 16:04:15
 * @FilePath: /myLearning/boostasio/coroutineServer/main.cpp
 */
#include <iostream>
#include <csignal>
#include "CServer.h"
#include <thread>
#include <mutex>


int main(){
    try{
        auto &pool = AsioIoServivePool::GetInstance();
        boost::asio::io_context io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context, &pool](const boost::system::error_code, int){
            io_context.stop();
            pool.Stop();
        });

    }catch(std::exception& e){
        
    }

    return 0;
}