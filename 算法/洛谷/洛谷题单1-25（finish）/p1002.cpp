/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 15:30:30
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 16:12:50
 * @FilePath: /myLearning/算法/洛谷/p1002.cpp
 */
#include <bits/stdc++.h>
using namespace std;
int dx[8] = {1,2,2,1,-1,-2,-2,-1}, dy[8] = {-2,-1,1,2,2,1,-1,-2};
long long f[25][25];
bool st[25][25];
int main(){
    int a, b, c, d;
    cin>>a>>b>>c>>d;
    for(int i = 0; i < 8; i++){
        int x = c + dx[i], y = d + dy[i];
        if(x >= 0 && y >= 0)
        st[x][y] = true;
    }
    st[c][d] = true;
    for(int i = 0; i <= a; i++){if(!st[i][0])f[i][0] = 1;else break;}
    for(int j = 0; j <= b; j++){if(!st[0][j])f[0][j] = 1;else break;};
    for(int i = 1; i <= a; i++){
        for(int j = 1; j <= b; j++){
            if(st[i][j])continue;
            else f[i][j] = f[i - 1][j] + f[i][j - 1];
        }
    }
    cout<<f[a][b]<<endl;
    return 0;
}