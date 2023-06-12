/*
 * @Author: zzzzztw
 * @Date: 2023-06-08 19:40:05
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-12 13:46:35
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/const.h
 */
#pragma once
#define MAX_LENGTH 1024 * 2
#define HEAD_LENGTH 2
#define MAX_RECVQUE 10000
#define MAX_SENDQUE 1000
#define HEAD_TOTAL_LEN 4
#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2

enum MSG_IDS{
    MSG_HELLO_WORD = 1001
};