/*
 * @Author: zzzzztw
 * @Date: 2023-09-17 17:05:21
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-17 17:17:04
 * @FilePath: /myLearning/算法/洛谷/p1170.cpp
 */
#include <bits/stdc++.h>
using namespace std;
int gcd(int a, int b){
    return b ? gcd(b, a%b) : a;
}
int main(){
    int n;
    cin>>n;
    while(n--){
        int x, y, a, b;
        cin>>x>>y>>a>>b;
        if(gcd(abs(x - a), abs(b - y)) > 1){
            cout<<"yes"<<endl;
        }
        else cout<<"no"<<endl;
    }
    return 0;
}