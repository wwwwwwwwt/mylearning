/*
 * @Author: zzzzztw
 * @Date: 2023-09-17 18:29:37
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-17 18:56:50
 * @FilePath: /myLearning/算法/洛谷/p1364.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 110;
vector<int>g[N];
int d[N];
int tot;
int sum[N], up[N], cnt[N];
int n;
int dfs(int u, int fa){
    cnt[u] = d[u];
    for(auto c : g[u]){
        if(c == fa)continue;
        cnt[u] += dfs(c, u);
    }
    return cnt[u];
}

void dfs1(int u, int fa){
    for(auto c: g[u]){
        if(c == fa)continue;
        dfs1(c, u);
        sum[u] += sum[c] + cnt[c];
    }
}

void dfs2(int u, int fa){
    for(auto c : g[u]){
        if(c == fa)continue;
        up[c] = up[u] + (tot - cnt[c]) + (sum[u] - sum[c]- cnt[c]);
        dfs2(c, u);
    }
}

int main(){
    cin>>n;
    for(int i = 1; i <= n; i++){
        int w, x, y;
        cin>>w>>x>>y;
        tot += w;
        d[i] = w;
        if(x != 0){
            g[i].push_back(x);
            g[x].push_back(i);
        }
        if(y != 0){
            g[i].push_back(y);
            g[y].push_back(i);
        }
    }
    dfs(1, 0);
    dfs1(1, 0);
    dfs2(1, 0);
    int res = 1e9;
    for(int i = 1; i <= n; i++){
        res = min(res, sum[i] + up[i]);
    }
    
    cout<<res<<endl;
    return 0;

}