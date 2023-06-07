/*
 * @Author: zzzzztw
 * @Date: 2023-06-07 18:27:11
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-07 18:35:01
 * @FilePath: /myLearning/算法/面试/记录最长的上升子序列.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 50;
int f[N], g[N];
int main(){
    vector<int>a = {5,4,3,2,1};
    int idx, res = 0;
    for(int i = 0; i < a.size(); i++){
        f[i] = 1, g[i] = -1;
        for(int j = i-1; j >= 0; j--){
            if(a[i] > a[j]){
                if(f[i] < f[j] + 1){
                    f[i] = f[j] + 1;
                    g[i] = j;
                }
            }
        }
        if(res < f[i]){
            res = f[i];
            idx = i;
        }
    }
    vector<int>b;
    while(idx >= 0){
        b.push_back(a[idx]);
        idx = g[idx];
    }
    reverse(b.begin(), b.end());
    for(int i = 0; i<b.size(); i++)cout<<b[i]<<" ";
    cout<<endl;
    return 0;
    
}