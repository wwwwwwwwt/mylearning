/*
 * @Author: zzzzztw
 * @Date: 2023-03-24 11:29:20
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-24 11:31:21
 * @FilePath: /cpptest/test.cpp
 */
#include <bits/stdc++.h>

using namespace std;

int main(){
    vector<int>a;
    a.push_back(1);
    a.push_back(1);
    a.push_back(1);
    a.push_back(1);
    
    a.push_back(1);
    a.push_back(1);
    a.push_back(1);
    a.push_back(1);

    vector<int>b(a.begin(), a.end() - 1);

    for(auto c:b)cout<<c;

    return 0;
}