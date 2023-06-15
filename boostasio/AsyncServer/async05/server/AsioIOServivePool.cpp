/*
 * @Author: zzzzztw
 * @Date: 2023-06-15 14:57:38
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-15 16:06:43
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/AsioIOServivePool.cpp
 */
#include "AsioIOServivePool.h"

using namespace std;

AsioIOServivePool::AsioIOServivePool(size_t size):_ioservives(size), _works(size) ,_nextIOService(0){
    for(size_t i = 0; i < size; i++){
         _works[i] =  unique_ptr<Work>(new Work(_ioservives[i]));
        
    }
    //遍历多个ioservice，创建多个线程，每个线程内部启动ioservice

    for (size_t i = 0; i < _ioservives.size(); i++)
    {
        //bug记录，这里按引用捕获&会段错误，因为i会变化
        _threads.emplace_back([this, i](){
            _ioservives[i].run();
        });
    }
    
}

AsioIOServivePool::~AsioIOServivePool(){
   // Stop();
    std::cout << "AsioIOservivePool destruct" << std::endl;
}

boost::asio::io_context& AsioIOServivePool::GetIOServive(){
    auto &Service = _ioservives[_nextIOService];
    _nextIOService++;
    if(_nextIOService == _ioservives.size()){
        _nextIOService = 0;
    }

    return Service;
}

void AsioIOServivePool::Stop(){
    for(auto &work : _works){
        work.reset();
    }

    for(auto &t : _threads){
        if(t.joinable()){
            t.join();
        }
    }
}