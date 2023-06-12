/*
 * @Author: zzzzztw
 * @Date: 2023-06-12 13:32:49
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-12 15:11:38
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/LogicSystem.h
 */
#pragma once
#include "Single.h"
#include "CSession.h"
#include "const.h"
#include <queue>
#include <thread>
#include <map>
#include <functional>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
typedef function<void(shared_ptr<CSession>, const short& msg_id, const string&msg_data)>FuncCallBack;

class LogicSystem:public SingleInstance<LogicSystem>{
friend class SingleInstance<LogicSystem>;

public:
    ~LogicSystem();
    void PostMsgToQue(shared_ptr<LogicNode>msg);
    void RegisterCallBacks();
    void HelloWorldCallBack(shared_ptr<CSession>, const short& msg_id, const string&msg_data);

private:
    LogicSystem();
    std::queue<shared_ptr<LogicNode>>_msg_que;
    std::mutex mtx;
    void DealMsg();
    std::condition_variable _cv;
    bool is_stop;
    std::map<short, FuncCallBack>_func_callback;
    std::thread _worker_thread;
};