/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 19:14:35
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 19:21:46
 * @FilePath: /myLearning/算法/洛谷/p1060.cpp
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 30;
int v[N], w[N];
ll f[30010];
int  main(){
    int n, m;
    cin>>n>>m;
    for(int i = 0; i < m; i++)cin>>v[i]>>w[i];
    for(int i = 0; i < m; i++){
        for(int j = n; j >= v[i]; j--){
            f[j] = max(f[j], f[j - v[i]] + v[i] * w[i]);
        }
    }
    cout<<f[n]<<endl;
    return 0;
}