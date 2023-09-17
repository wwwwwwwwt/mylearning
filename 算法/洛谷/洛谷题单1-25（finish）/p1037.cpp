/*
 * @Author: zzzzztw
 * @Date: 2023-09-16 19:03:49
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-16 22:24:34
 * @FilePath: /myLearning/算法/洛谷/p1037.cpp
 */
#include <bits/stdc++.h>
using namespace std;
int f[11][11];
int d[11];
using ll = __uint128_t;
void out(ll x){//int128输出要自己写
	if(x>9)out(x/10);
	putchar(x%10+48);
}
int main(){
    ll res = 1;
    string num;
    int k;
    cin>>num>>k;
    for(int i = 0; i < k; i++){
            int x, y;
            cin>>x>>y;
            f[x][y] = 1;
    }
    for(int k = 0; k <= 9; k++){
        for(int i = 0; i <= 9; i++){
            for(int j = 0; j <= 9; j++){
                if(f[i][k] && f[k][j])f[i][j] = 1;
            }
        }
    }
    for(int i = 0; i <= 9; i++){
        d[i] = 1;
        for(int j = 0; j <= 9; j++)if(j!=i && f[i][j])d[i]++;
    }
    for(int i = 0; i < num.size(); i++)res *= d[num[i] - '0'];
    out(res);
    return 0;
    
}