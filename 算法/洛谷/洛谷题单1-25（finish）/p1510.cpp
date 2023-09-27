/*
 * @Author: zzzzztw
 * @Date: 2023-09-26 20:30:59
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-26 20:41:54
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1510.cpp
 */
#include <bits/stdc++.h>
using namespace std;
int a[10010], w[10010];
int f[10010];
int main(){
    int v, n, c;
    cin>>v>>n>>c;
    for(int i = 1; i <= n; i++){
        cin>>a[i]>>w[i];
    }
    for(int i = 1; i <= n; i++){
        for(int j = c; j >= w[i]; j--){
            f[j] = max(f[j], f[j - w[i]] + a[i]);
        }
    }
    int res = 0;
    for(int i = 0; i <= c; i++){
        if(f[i]>=v){
            res = i;
            break;
        }
    }
    if(f[c] < v)cout<<"Impossible"<<endl;
    else cout<<c- res<<endl;
    return 0;
}