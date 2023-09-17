/*
 * @Author: zzzzztw
 * @Date: 2023-09-17 16:35:30
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-17 17:01:32
 * @FilePath: /myLearning/算法/洛谷/p1192.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 1e5+10;
unsigned long long f[N];
const int Mod =100003;
int main(){
    int n, k;
    cin>>n>>k;
    f[0] = 1;
    f[1] = 1;
    for(int i = 2; i <= k; i++){
        for(int j = 1; j <= i; j++){
            f[i] = (f[i] + f[i - j]) % Mod;
        }
    }
    for(int i = k + 1; i <= n; i++){
        for(int j = 1; j <= k; j++){
            f[i] = (f[i] + f[i - j]) % Mod;
        }
    }
    cout<<f[n]%Mod<<endl;
    return 0;
}