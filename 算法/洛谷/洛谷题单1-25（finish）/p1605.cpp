/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 20:14:57
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 21:15:35
 * @FilePath: /myLearning/算法/洛谷/p1605.cpp
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 20;
int g[N][N];
int n, m, t;
int a, b , c, d;
int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
bool st[N][N];
ll res;
void dfs(int curx, int cury){
    if(curx == c && cury == d){
        res++;
        return;
    }
    ll res = 0;
    for(int i = 0; i < 4; i++){
        int x = curx + dx[i], y = cury + dy[i];
        if(x > 0 && x <= n && y > 0 && y <= m && g[x][y]!= 1 && !st[x][y]){
            st[curx][cury] = true;
            dfs(x, y);
            st[curx][cury] = false;
        }
    }
    
}
int main(){
    cin>>n>>m>>t;
    cin>>a>>b>>c>>d;
    for(int i = 0; i < t; i++){
        int x, y;
        cin>>x>>y;
        g[x][y] = 1;
    }
    st[a][b] = true;
    dfs(a, b);
    cout<<res<<endl;
    return 0;
}