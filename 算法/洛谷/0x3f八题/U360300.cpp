/*
 * @Author: zzzzztw
 * @Date: 2023-09-15 08:30:03
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-15 08:44:06
 * @FilePath: /myLearning/算法/洛谷/U360300.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 200010;
int a[N];
long long s[N];
int main(){
    int n;
    cin>>n;
    for(int i = 1; i <= n; i++)scanf("%d", &a[i]);
    for(int i = 1; i <= n / 2; i++){
        s[i] = n - (i - 1) * 2 + s[i - 1];
    }
    if(n % 2 == 1)s[n / 2 + 1] = s[n / 2] + 1;
    for(int i = 1; i <= n / 2; i++){
        s[n - i + 1] = s[i];
    }
    long long sum = 0;
    for(int i = 1; i <= n; i++)sum += 1ll * s[i] * a[i];
    cout<<sum<<endl;
    return 0;
}