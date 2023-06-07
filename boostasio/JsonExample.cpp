/*
 * @Author: zzzzztw
 * @Date: 2023-06-07 19:34:19
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-07 19:37:54
 * @FilePath: /myLearning/test.cpp
 */
#include <iostream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

int main(){
    Json::Value root;
    root["id"] = 1001;
    root["data"] = "hello world";
    std::string request = root.toStyledString();
    std::cout<<"request is "<<request<<std::endl;

    Json::Value root2;
    Json::Reader reader;
    reader.parse(request, root2);
    std::cout<<"msg id is "<<root2["id"] <<" msg is "<<root2["data"]<<std::endl;
    return 0;
}