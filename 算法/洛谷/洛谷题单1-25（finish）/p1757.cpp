/*
 * @Author: zzzzztw
 * @Date: 2023-10-07 11:34:07
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-07 11:43:46
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1757.cpp
 */
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
vector<pii>g[1010];
long long f[10010];
int main(){
    int n, v, t = 0;
    cin>>v>>n;
    for(int i = 0; i < n; i++){
        int a, b, c;
        cin>>a>>b>>c;
        t = max(t, c);
        g[c].push_back({a, b});
    }
    for(int i = 1; i <= t; i++){
        for(int j = v; j >= 0; j--){
            for(auto c : g[i]){
                if(j >= c.first){
                    f[j] = max(f[j], f[j - c.first] + c.second);
                }
            }
        }
    }
    cout<<f[v]<<endl;
    return 0;
}