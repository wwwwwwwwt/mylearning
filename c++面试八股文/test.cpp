/*
 * @Author: zzzzztw
 * @Date: 2023-04-06 13:13:34
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-06 13:15:35
 * @FilePath: /cpptest/c++面试八股文/test.cpp
 */
#include <bits/stdc++.h>
using namespace std;

int main(){
    char a = '1';

    char *p = &a;
    char& r = a;

    cout<<sizeof p <<"   "<<sizeof r<<endl;
    return 0;
}