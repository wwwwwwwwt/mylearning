/*
 * @Author: zzzzztw
 * @Date: 2023-06-16 12:42:43
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-16 15:08:30
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/AsioThreadPool.h
 */
#pragma once
#include "Single.h"
#include <vector>
#include <thread>
#include <boost/asio.hpp>

class AsioThreadPool:public SingleInstance<AsioThreadPool>
{
    friend class SingleInstance<AsioThreadPool>;
public:
    ~AsioThreadPool(){std::cout<<"pool destruct"<<std::endl;};
    AsioThreadPool(const AsioThreadPool&) = delete;
    AsioThreadPool& operator=(const AsioThreadPool&) = delete;
    boost::asio::io_context& GetIOService();
    void Stop();
private:
    AsioThreadPool(int threadNum = std::thread::hardware_concurrency());
    boost::asio::io_context _service;
    std::unique_ptr<boost::asio::io_context::work>_workPtr;
    std::vector<std::thread>_threads;
};