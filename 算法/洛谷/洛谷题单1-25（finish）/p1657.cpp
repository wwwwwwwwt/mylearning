/*
 * @Author: zzzzztw
 * @Date: 2023-10-07 11:20:27
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-07 11:29:36
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1657.cpp
 */
#include <bits/stdc++.h>
using namespace std;
vector<int>g[25];
int n;
int res;
bool st[25];
void dfs(int i){
    if(i == n + 1)return;
    for(auto c : g[i]){
        if(!st[c]){
            st[c] = true;
            if(i == n - 1)res++;
            dfs(i + 1);
            st[c] = false;
        }
    }
}
int main(){

    cin>>n;
    for(int i = 0; i < n; i++){
        int a, b;
        cin>>a>>b;
        g[i].push_back(b);
        g[i].push_back(a);
    }
    dfs(0);
    cout<<res<<endl;
    return 0;
}