/*
 * @Author: zzzzztw
 * @Date: 2023-10-07 10:44:26
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-07 12:06:27
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1636.cpp
 */
#include <bits/stdc++.h>
using namespace std;
vector<int>d;
int main(){
    int n, m;
    cin>>n>>m;
    d.resize(n + 1);
    for(int i = 0; i < m; i++){
        int a, b;
        cin>>a>>b;
        d[a]++, d[b]++;
    }
    int cnt = 0;
    for(int i = 1; i <= n; i++){
        if((d[i])% 2 == 1)cnt++;
    }
    if(!cnt || cnt == 2)cout<<1<<endl;
    else cout<<cnt / 2 <<endl;
    return 0;
}