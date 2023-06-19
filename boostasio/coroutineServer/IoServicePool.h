#pragma once
#include <vector>
#include <boost/asio.hpp>
#include <memory>
#include <thread>
class AsioIoServivePool
{
public:
    using IOService = boost::asio::io_context;
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;
    ~AsioIoServivePool();
    AsioIoServivePool(const AsioIoServivePool&) = delete;
    AsioIoServivePool& operator=(const AsioIoServivePool&) = delete;
    boost::asio::io_context& GetIOService();
    void Stop();
    static AsioIoServivePool& GetInstance();
private:
    AsioIoServivePool(std::size_t size = std::thread::hardware_concurrency());
    std::vector<IOService>_ioservice;
    std::vector<WorkPtr>_works;
    std::vector<std::thread>_threads;
    std::size_t nextIOService;
};