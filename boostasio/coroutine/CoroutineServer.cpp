/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 19:10:42
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-18 19:56:59
 * @FilePath: /myLearning/boostasio/coroutine/CoroutineServer.cpp
 */
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

awaitable<void> echo(tcp::socket socket){
    try{
        char data[1024];
        for(;;){
            std::size_t n =  co_await socket.async_read_some(boost::asio::buffer(data), use_awaitable);
            co_await async_write(socket, boost::asio::buffer(data), use_awaitable);
        }

    }catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
}

awaitable<void> listener(){
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), 10086});
    for(;;){
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        co_spawn(executor, echo(std::move(socket)), detached);
    }
}

int main(){
    try
    {
        boost::asio::io_context io_c(1);
        boost::asio::signal_set signals(io_c, SIGINT, SIGTERM);
        signals.async_wait([&](const boost::system::error_code, int){
           io_c.stop(); 
        });
        co_spawn(io_c, listener(), detached);
        io_c.run();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}