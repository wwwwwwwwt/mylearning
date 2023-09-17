/*
 * @Author: zzzzztw
 * @Date: 2023-09-17 16:32:23
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-17 16:34:33
 * @FilePath: /myLearning/算法/洛谷/p1164.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10;
long long a[N],f[N];

int main(){
    int n,m;
    cin>>n>>m;
    for(int i = 0; i < n; i++)cin>>a[i];
    f[0] = 1;
    for(int i = 0; i < n; i++){
        for(int j = m; j >= a[i]; j--){
            f[j] += f[j - a[i]];
        }
    }
    cout<<f[m]<<endl;
    return 0;
}