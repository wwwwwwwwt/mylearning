/*
 * @Author: zzzzztw
 * @Date: 2023-09-26 19:15:01
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-26 19:42:59
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1403.cpp
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int n;
    cin>>n;
    ll res = 0;
    for(int i = 1; i <= n; i++)res += n / i;
    cout<<res<<endl;
    return 0;
}