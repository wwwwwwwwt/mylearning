/*
 * @Author: zzzzztw
 * @Date: 2023-06-15 12:31:57
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-15 17:40:29
 * @FilePath: /myLearning/算法/面试/凑硬币.cpp
 */


// 凑硬币，amout是总数，coin_num是最多可用硬币数，有1，2，5，10四种面值，可以凑成返回组合，凑不成返回空数组
//如 14，4，返回[10, 2, 2]，表示一个10，两个2
//如8， 2，返回[]，因为凑不出来
#include <bits/stdc++.h>
using namespace std;
const int N = 1010;
bool f[N][N]; // 容量 数量
using pii = pair<int, int>;
pii pos[N][N];
int main(){
    memset(f, 0, sizeof f);
    int n = 48, m = 7;
    vector<int>a = {5,4,7,10};
    sort(a.begin(), a.end());
    if(n < a[0])cout<<" "<<endl;
    f[0][0] = true;
    for(int i = 0; i < a.size(); i++){
        for(int j = a[i]; j <= n; j++){
            for(int k = 1; k <= m; k++){
                if(f[j][k])continue;
                f[j][k] |=  f[j - a[i]][k-1];
                if(f[j][k]){
                    pos[j][k] = {i, j-a[i]};
                }
            }
        }
    }
    int idx = -1;
    for(int i = 1; i <= m; i++){
        if(f[n][i] == true){
            idx = i;
            break;
        }
    }
    
    if(idx == -1)return 0;
    auto target = pos[n][idx];
    while(idx){
        cout<<a[target.first]<<" ";
        idx--;
        target = pos[target.second][idx];
    }
    cout<<endl;
    return 0;
}