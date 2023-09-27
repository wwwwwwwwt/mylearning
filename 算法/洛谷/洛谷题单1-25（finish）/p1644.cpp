/*
 * @Author: zzzzztw
 * @Date: 2023-09-26 19:44:08
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-26 19:48:10
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1644.cpp
 */
#include <bits/stdc++.h>
using namespace std;
int n, m;
int res;
int dx[4] = {-2,-1,1,2}, dy[4] = {1,2,2,1};
void dfs(int a, int b){
    if(a == n && b == m){
        res++;
        return;
    }
    for(int i = 0; i < 4; i++){
        int x = a + dx[i], y = b + dy[i];
        if(x >= 0 && x <= n && y >= 0 && y <= m){
            dfs(x, y);
        }
    }
}
int main(){

    cin>>n>>m;
    dfs(0, 0);
    cout<<res<<endl;
    return 0;
}