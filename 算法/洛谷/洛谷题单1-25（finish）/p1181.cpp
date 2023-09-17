/*
 * @Author: zzzzztw
 * @Date: 2023-09-17 17:22:08
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-17 17:43:43
 * @FilePath: /myLearning/算法/洛谷/p1181.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int a[N];
bool check(int a[], int n, int M, int mid){
    int sum = 0;
    int cnt = 1;
    for(int i = 0; i < n; i++){
        if(sum + a[i] > M){
            cnt++;
            sum = a[i];
        }else{
            sum += a[i];
        }
    }
    return cnt > mid;
}
int main(){
    int n, m;
    cin>>n>>m;
    for(int i = 0; i < n; i++)cin>>a[i];
    int l = 1, r = 1e9;
    while(l < r){
        int mid = l + r>> 1;
        if(check(a, n, m, mid))l = mid + 1;
        else r = mid;
    }
    cout<<r<<endl;
    return 0;
}