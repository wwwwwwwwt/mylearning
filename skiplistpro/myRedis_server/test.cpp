/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 13:49:12
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-20 21:32:18
 * @FilePath: /cpptest/test.cpp
 */
#include "./skiplistpro/command.h"
#include "skiplist.h"

using namespace std;
int main(){
   /* string str = "get 456 56";
    Command a(str);
    a.is_valid_command();*/
    Skiplist<int,int> a(32);

    a.insert_element(11,1);
   a.insert_element(51,1);
    a.insert_element(21,1);
    a.insert_element(31,1);
    a.insert_element(61,1);
    a.insert_element(41,1);
    a.insert_element(91,1);
    a.insert_element(101,1);
    a.insert_element(5,1);
    a.insert_element(3,1);
    a.insert_element(1,1);
    a.insert_element(563,1);
    a.insert_element(500,1);
    a.insert_element(1000,1);

    a.insert_element(1000,2);

    a.display_list();

    a.delete_element(51);

    a.delete_element(5);
    a.delete_element(3);
    a.delete_element(1);
    a.delete_element(563);
    a.delete_element(500);
    a.delete_element(1000);


    a.display_list();

    return 0;
}