/*
 * @Author: zzzzztw
 * @Date: 2023-06-15 14:19:34
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-15 15:45:59
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/AsioIOServivePool.h
 */

#pragma once
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include "Single.h"
class AsioIOServivePool : public SingleInstance<AsioIOServivePool>
{
    friend class SingleInstance<AsioIOServivePool>;
public:
    using IOServive = boost::asio::io_context;
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;
    ~AsioIOServivePool();
    AsioIOServivePool(const AsioIOServivePool&) = delete;
    AsioIOServivePool& operator= (const AsioIOServivePool&) = delete;

    // 使用round-robin轮询的方式返回一个io_context
    boost::asio::io_context& GetIOServive();
    void Stop();
private:
    AsioIOServivePool(std::size_t size = std::thread::hardware_concurrency());
    std::vector<IOServive>_ioservives;
    std::vector<WorkPtr>_works;
    std::vector<std::thread>_threads;
    std::size_t _nextIOService;
};