/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 21:56:44
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-18 22:25:39
 * @FilePath: /cpptest/boostasio/coroutineServer/IoServicePool.cpp
 */
#include "IoServicePool.h"
#include <iostream>
using namespace std;

AsioIoServivePool::AsioIoServivePool(std::size_t size):_ioservice(size),
_works(size),nextIOService(0){
    for(size_t i = 0; i < size; i++){
        _works[i] = std::unique_ptr<Work>(new Work(_ioservice[i]));
    }

    for(size_t i = 0; i < size; i++){
        _threads.emplace_back([this, i](){
            _ioservice[i].run();
        });
    }
}

AsioIoServivePool::~AsioIoServivePool(){
    std::cout<<"AsioIOServivePool destruct"<<std::endl;
}

boost::asio::io_context& AsioIoServivePool::GetIOService(){
    auto& service = _ioservice[nextIOService++];
    if(nextIOService == _ioservice.size()){
        nextIOService = 0;
    }
    return service;
}

void AsioIoServivePool::Stop(){
    for(auto& work : _works){
        work.reset();
    }

    for(auto &t : _threads){
        if(t.joinable()){
            t.join();
        }
    }
}

AsioIoServivePool& AsioIoServivePool::GetInstance(){
    static AsioIoServivePool instance;
    return instance;
}