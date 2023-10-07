/*
 * @Author: zzzzztw
 * @Date: 2023-10-07 11:51:35
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-07 12:04:19
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1652.cpp
 */
#include <bits/stdc++.h>
using namespace std;
int x[55], y[55], r[55];
int a, b, c, d;
int n;
int dis(int g, int h, int j, int k){
    return (g - j) * (g - j) + (h - k) * (h - k);
}
int main(){
    cin>>n;
    for(int i = 0 ; i < n; i++){
        cin>>x[i];
    }
    for(int i = 0; i < n; i++){
        cin>>y[i];
    }
    for(int i = 0; i < n; i++){
        cin>>r[i];
    }
    cin>>a>>b>>c>>d;
    unordered_map<int, int>mp;
    for(int i = 0; i < n; i++){
        if(dis(a, b, x[i], y[i]) < r[i] * r[i])mp[i]++;
    }
    for(int i = 0; i < n; i++){
        if(dis(c, d, x[i], y[i]) < r[i] * r[i])mp[i]--;
    }
    int res = 0;
    for(auto c : mp){
        if(c.second != 0)res++;
    }
    cout<<res<<endl;
    return 0;

}