/*
 * @Author: zzzzztw
 * @Date: 2023-09-16 09:27:31
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-16 09:33:25
 * @FilePath: /myLearning/算法/洛谷/p1094.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 30010;
int a[N];

int main(){
    int m;
    cin>>m;
    int n;
    cin>>n;
    for(int i = 0; i < n; i++)cin>>a[i];
    sort(a, a + n);
    int  res = 0;
    int l = 0, r = n - 1;
    while(l < r){
        if(a[l] + a[r] <= m){
            res++;
            l++,r--;
        }
        else{
            r--;
            res++;
        }
    }
    if(l == r)res++;
    cout<<res<<endl;
    return 0;
}