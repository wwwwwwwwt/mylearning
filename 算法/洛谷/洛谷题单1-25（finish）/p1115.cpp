/*
 * @Author: zzzzztw
 * @Date: 2023-09-16 08:45:29
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-16 09:39:18
 * @FilePath: /myLearning/算法/洛谷/p1115.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 200010;
int a[N], f[N];

int main(){
    int n;
    cin>>n;
    for(int i = 0; i < n; i++)scanf("%d", &a[i]);
    f[0] = a[0];
    int res = INT_MIN;
    for(int i = 1; i < n; i++){
        f[i] = max(f[i - 1] + a[i], a[i]);
        res = max(f[i], res);
    }    
    cout<<res<<endl;
    return 0;
}