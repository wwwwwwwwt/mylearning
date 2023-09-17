/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 19:51:21
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 19:56:12
 * @FilePath: /myLearning/算法/洛谷/p1097.cpp
 */
#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0),cin.tie(0), cout.tie(0);
    map<int, int>mp;
    int n;
    cin>>n;
    for(int i = 0; i < n; i++){
        int x;
        cin>>x;
        mp[x]++;
    }
    for(auto c : mp){
        cout<<c.first <<" "<<c.second<<endl;
    }
    return 0;
}