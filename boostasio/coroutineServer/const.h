/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 21:14:01
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-18 21:16:54
 * @FilePath: /cpptest/boostasio/coroutineServer/const.h
 */
#pragma once

#define MAX_LENGTH  1024 * 2
#define HEAD_TOTAL_LEN  4
#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2
#define MAX_RECVQUE 10000
#define MAX_SENDQUE 1000

enum MSG_IDS{
    MSG_HEAD_WORLD = 1001
}