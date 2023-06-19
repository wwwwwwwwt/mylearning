/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 21:13:27
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-18 21:41:57
 * @FilePath: /cpptest/boostasio/coroutineServer/msgnode.h
 */
#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include "const.h"

class MsgNode{
public:
    friend class SendNode;
    MsgNode(short max_len):_total_len(max_len),_cur_len(0){
        _data = new char[_total_len + 1];
        _data[_total_len] = '\0';
    }
    ~MsgNode(){
        std::cout<<"destruct MsgNode"<<std::endl;
        delete[] _data;
    }

    void Clear(){
        ::memset(_data, 0, sizeof _data);
        _cur_len = 0;
    }

private:
    short _cur_len;
    short _total_len;
    char* _data;
};

class RecvNode:public MsgNode
{
public:
    RecvNode(short max_len, short msg_id);

private:
    short _msg_id;
};


class SendNode:public MsgNode
{
public:
    SendNode(const char* msg, short max_len, short msg_id);
private:
    short _msg_id;
};