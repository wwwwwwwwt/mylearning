/*
 * @Author: zzzzztw
 * @Date: 2023-02-24 16:57:15
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-23 21:10:11
 * @FilePath: /cpptest/smartptr/test.cpp
 */
#include <iostream>
#include "myshareptr.h"
#include <memory>

class test
{
public:

    test(int a,int b):a(a),b(b){};
    ~test(){};

    void print(){
        std::cout<<"a + b = "<<a+b<<"\n";
    }


private:
    int a;
    int b;

};


int main(){
    Myshared_ptr<char>a(new char('a'));
    Myshared_ptr<char>b(a);
    Myshared_ptr<char>c;
    c = a;
    c = b;
    std::cout <<b.user_count()<<std::endl;
    Myshared_ptr<char>d(new char('d'));
    c = d;

    std::shared_ptr<test>p = std::make_shared<test>(4,5);

    p.get()->print();

    std::cout<<*b<<std::endl;
    std::cout<<*c<<std::endl;
    std::cout <<b.user_count()<<std::endl;
    std::cout <<d.user_count()<<std::endl;

    return 0;


}