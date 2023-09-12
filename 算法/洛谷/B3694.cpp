/*
 * @Author: zzzzztw
 * @Date: 2023-09-12 09:08:12
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-12 09:16:28
 * @FilePath: /myLearning/算法/洛谷/B3694.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int s[N];
int main(){
    int T;
    cin>>T;
    while(T--){
        int n;
        cin>>n;
        map<int, int>mp;

        for(int i = 0; i < n; i++){
            int x;
            cin>>x;
            s[i] = x;   
            mp[x]++;
        }
        vector<int>a;
        for(auto x : mp){
            a.push_back(x.first);
        }
        for(int i = 0; i < n; i++){
            auto res = lower_bound(a.begin(), a.end(), s[i]) - a.begin() + 1;
            cout<<res<<" ";
        }
        cout<<endl;
    }
    return 0;
}