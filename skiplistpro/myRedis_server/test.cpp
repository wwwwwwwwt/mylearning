/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 13:49:12
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-16 14:10:55
 * @FilePath: /cpptest/skiplistpro/myRedis_server/test.cpp
 */
#include "./command.h"

int main(){
    string str = "get 456 56";
    Command a(str);
    a.is_valid_command();

    return 0;
}