/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 20:11:35
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 20:13:33
 * @FilePath: /myLearning/算法/洛谷/p1090.cpp
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main(){
    int n;
    cin>>n;
    priority_queue<int, vector<int>,greater<int>>q;
    for(int i = 0; i < n; i++){
        int x;
        cin>>x;
        q.push(x);
    }
    ll res = 0;
    while(q.size() >= 2){
        auto a = q.top();
        q.pop();
        auto b = q.top();
        q.pop();
        res += a + b;
        q.push(a + b);
    }
    cout<<res<<endl;
    return 0;
}