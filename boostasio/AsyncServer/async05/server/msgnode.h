/*
 * @Author: zzzzztw
 * @Date: 2023-06-08 19:00:55
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-08 20:30:42
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/msgnode.h
 */
#pragma once
#include <string>
#include <boost/asio.hpp>
#include <iostream>
#include "const.h"
using boost::asio::ip::tcp;

class MsgNode
{
public:
    MsgNode(short max_len) :_total_len(max_len), _cur_len(0) {
        _data = new char[_total_len + 1]();
        _data[_total_len] = '\0';
    }
    ~MsgNode() {
        std::cout << "destruct MsgNode" << std::endl;
        delete[] _data;
    }
    void Clear() {
        ::memset(_data, 0, _total_len);
        _cur_len = 0;
    }
    short _cur_len;
    short _total_len;
    char* _data;
};
class RecvNode :public MsgNode {
public:
    RecvNode(short max_len, short msg_id);
private:
    short _msg_id;
};
class SendNode:public MsgNode {
public:
    SendNode(const char* msg,short max_len, short msg_id);
private:
    short _msg_id;
};