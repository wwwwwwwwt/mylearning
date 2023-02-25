/*
 * @Author: zzzzztw
 * @Date: 2023-02-24 16:57:15
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-02-24 17:19:51
 * @FilePath: /zhang/smartptr/test.cpp
 */
#include <iostream>
#include "myshareptr.h"

int main(){
    Myshared_ptr<char>a(new char('a'));
    Myshared_ptr<char>b(a);
    Myshared_ptr<char>c;
    c = a;
    c = b;
    std::cout <<b.user_count()<<std::endl;
    Myshared_ptr<char>d(new char('d'));
    c = d;

    std::cout<<*b<<std::endl;
    std::cout<<*c<<std::endl;
    std::cout <<b.user_count()<<std::endl;
    std::cout <<d.user_count()<<std::endl;

    return 0;


}