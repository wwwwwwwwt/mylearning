/*
 * @Author: zzzzztw
 * @Date: 2023-06-08 19:00:00
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-16 15:50:37
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
//#include "AsioIOServivePool.h"
#include "AsioThreadPool.h"
using namespace std;

volatile bool isstop = false;
std::condition_variable _cv;
std::mutex mtx_;
int main(){

    try{
        auto pool = AsioIOServivePool::GetInstance();
      // auto pool = AsioThreadPool::GetInstance();
        boost::asio::io_context ioc;

        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&](const boost::system::error_code, int){
            ioc.stop();
            pool->Stop();
        
        });
        CServer s(ioc, 10086);
       // io_context io.run(), 开始轮询
       // 启动异步事件循环。这个事件循环会在需要处理的异步操作完成时自动触发相关的回调函数，以及在没有待处理事件时休眠以节省系统资源。
        ioc.run();
    /*    {
            //防止子线程还没执行完主线程就退出，所以主线程在这里需要卡住
            unique_lock<mutex>locker(mtx_);
            while(!isstop){
                _cv.wait(locker);
            }
        }
    */
    }catch(std::exception& e){
        std::cout<<"err ! err is "<<e.what()<<std::endl;
    }

}


/*
using namespace std;
bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main()
{
	try {
		auto pool = AsioThreadPool::GetInstance();
		boost::asio::io_context io_context;
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([pool,&io_context](auto, auto) {
			io_context.stop();
			pool->Stop();
			std::unique_lock<std::mutex> lock(mutex_quit);
			bstop = true;
			cond_quit.notify_one();
			});
		CServer s(pool->GetIOService(), 10086);
		{
			std::unique_lock<std::mutex> lock(mutex_quit);
			while (!bstop) {
				cond_quit.wait(lock);
			}
		}
		
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << endl;
	}

}*/
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