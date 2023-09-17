/*
 * @Author: zzzzztw
 * @Date: 2023-09-16 17:22:36
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-16 17:40:06
 * @FilePath: /myLearning/算法/洛谷/p1102.cpp
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    map<ll, ll>mp;
    ll res = 0;
    int n, c;
    cin>>n>>c;
    for(int i = 0; i < n; i++){
        ll x;
        cin>>x;
        mp[x]++;
    }
    for(auto x : mp){
       ll b = x.first, cnt = x.second;
       if(mp.find(c + b) != mp.end())res += 1ll * mp[1ll*c + b] * cnt;
    }
    cout<<res<<endl;
    return 0;
}