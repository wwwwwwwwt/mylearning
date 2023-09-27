/*
 * @Author: zzzzztw
 * @Date: 2023-09-26 19:50:25
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-26 20:30:25
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1122.cpp
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
vector<int>g[32010];
ll f[32010];

ll res;
ll dfs(int u, int fa){
    ll val = f[u];
    ll temp = 0;
    for(auto c : g[u]){
        if(c != fa){
            ll cnt = dfs(c, u);
            temp += max(cnt, 1ll*0);
        }
    }
    res = max(res, temp + val);
    return temp + val;
}
int main(){
    int n;
    cin>>n;
    for(int i = 1; i <= n; i++)cin>>f[i];
    for(int i = 1; i < n; i++){
        int a, b;
        cin>>a>>b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    res = LLONG_MIN;
    dfs(1, -1);
    cout<<res<<endl;
    return 0;
}