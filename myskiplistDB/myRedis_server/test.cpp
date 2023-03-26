/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 13:49:12
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-21 16:50:41
 * @FilePath: /cpptest/skiplistpro/myRedis_server/test.cpp
 */
#include "./command.h"
#include "skiplist.h"

#include <thread>

using namespace std;
int main(){
   /* string str = "get 456 56";
    Command a(str);
    a.is_valid_command();*/
    Skiplist<string,string> a(32);

 //   a.insert_element("LSJ","1");
    thread t1(&Skiplist<string,string>::insert_element,&a,"LSJ","1");
    cout<< a.size() <<endl;
    a.insert_element("ZTW","100");
    a.insert_element("WGL","10");
       cout<< a.size() <<endl;
    t1.join();
    a.display_list();

    a.dump_file();

    return 0;
}