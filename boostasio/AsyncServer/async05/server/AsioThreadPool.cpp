/*
 * @Author: zzzzztw
 * @Date: 2023-06-16 13:01:15
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-16 14:55:28
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/AsioThreadPool.cpp
 */
#include "AsioThreadPool.h"

AsioThreadPool::AsioThreadPool(int threadNums):_workPtr(new boost::asio::io_context::work(_service)){
    std::cout<<threadNums<<std::endl;
    for(int i = 0; i < threadNums; i++){
        _threads.emplace_back([this](){
            _service.run();
        });
    }
}

boost::asio::io_context& AsioThreadPool::GetIOService(){
    return _service;
}

void AsioThreadPool::Stop(){
    _workPtr.reset();
    for(auto &t : _threads){
        if(t.joinable()){
            t.join();
        }
    }
}