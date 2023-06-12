/*
 * @Author: zzzzztw
 * @Date: 2023-06-12 14:24:05
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-12 16:56:38
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/LogicSystem.cpp
 */
#include "LogicSystem.h"
using namespace std;

LogicSystem::LogicSystem():is_stop(false){
    RegisterCallBacks();
    _worker_thread = std::thread(&LogicSystem::DealMsg, this);
}

void LogicSystem::RegisterCallBacks(){
    _func_callback[MSG_HELLO_WORD] = std::bind(&LogicSystem::HelloWorldCallBack, this, std::placeholders::_1,
        std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::HelloWorldCallBack(shared_ptr<CSession>session, const short& msg_id, const string&msg_data){
    Json::Reader reader;
    Json::Value root;
    reader.parse(msg_data, root);
    std::cout<<"receive id is "<<root["id"].asInt()<<" msg data is "<<root["data"].asString()<<std::endl;

    root["data"] = "server has receive msg, msg is " +  root["data"].asString();
    std::string return_str = root.toStyledString();
    session->Send(return_str, root["id"].asInt());
    
}

//工作线程的工作逻辑，消费者逻辑
void LogicSystem::DealMsg(){
    for(;;){
        std::unique_lock<mutex>locker(mtx);


        //判断条件变量为空就等待
        _cv.wait(locker, [&](){
            return !_msg_que.empty() ||is_stop;
        });
        

        //如果为关闭状态，取出逻辑队列所有数据并退出循环
        if(is_stop){
            while(!_msg_que.empty()){
                auto it = _msg_que.front();
                
                cout<<"msg id is "<<it->_recvnode->_msg_id<<endl;
                auto callback_it = _func_callback.find(it->_recvnode->_msg_id);
                if(callback_it == _func_callback.end()){
                    _msg_que.pop();
                    continue;
                }else{
                    callback_it->second(it->_session, it->_recvnode->_msg_id, std::string(it->_recvnode->_data, it->_recvnode->_cur_len));
                     _msg_que.pop();
                }
            }
            break;
        }
        // 如果没关闭，队列有数据
        auto it = _msg_que.front();   
        cout<<"msg id is "<<it->_recvnode->_msg_id<<endl;
        auto callback_it = _func_callback.find(it->_recvnode->_msg_id);
        if(callback_it == _func_callback.end()){
            _msg_que.pop();
            continue;
        }else{
            callback_it->second(it->_session, it->_recvnode->_msg_id, std::string(it->_recvnode->_data, it->_recvnode->_cur_len));
            _msg_que.pop();
        }
    }
}

// push 生产者逻辑
void LogicSystem::PostMsgToQue(shared_ptr<LogicNode> msg){
   std::unique_lock<mutex>locker(mtx);
    _msg_que.push(msg);
    if(_msg_que.size() >= 1)_cv.notify_all();
}

LogicSystem::~LogicSystem(){
    is_stop = true;
    _cv.notify_all();
    if(_worker_thread.joinable()){
        _worker_thread.join();
    }
}