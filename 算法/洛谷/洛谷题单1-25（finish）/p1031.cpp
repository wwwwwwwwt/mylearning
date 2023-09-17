/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 18:09:56
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 18:24:01
 * @FilePath: /myLearning/算法/洛谷/p1031.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 110;
using ll = long long;
ll a[N];
int main(){
    int n;
    cin>>n;
    ll res = 0;
    for(int i = 1; i <= n; i++){
        cin>>a[i];
        res += a[i];
    }
    ll averge = res / n;
    int ans = 0;
    for(int i = 1; i <= n; i++){
        if(a[i] < averge){
            a[i + 1] -= averge - a[i];
            ans++;
        }
        else if(a[i] > averge) {
            a[i + 1] +=  a[i] - averge;
            ans++;
        }
    }
    cout<<ans<<endl;
    return 0;
}