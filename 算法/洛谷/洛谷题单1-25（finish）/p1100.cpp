/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 19:23:43
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 19:49:52
 * @FilePath: /myLearning/算法/洛谷/p1100.cpp
 */
#include <bits/stdc++.h>
using namespace std;

int main(){
    unsigned int n;
    cin>>n;
    cout<<(n >> 16) + (n << 16)<<endl;
    return 0;
}