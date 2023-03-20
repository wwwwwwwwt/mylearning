/*
 * @Author: zzzzztw
 * @Date: 2023-03-17 11:58:45
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-17 12:41:09
 * @FilePath: /cpptest/算法/面试/count.cpp
 */

/*在某2D游戏中，有一个长宽分别为n，m格的长方形栅格地图，# 游戏玩家起始位置在第i行，第i列的格子里，每一步会在上下左右四个方向中随机选择一个方向，走一步。
 求解:玩家走了x步之后，全程没有超出地图范围的概率是多少?
注:实现函数计算上述问题，n，m，i，j，x等作为函数输入参数。*/

//思路 力扣576，动态规划从边界反推到初始点，看有多少种方法能走到

#include <bits/stdc++.h>
using namespace std;
const int N = 10;
uint64_t f[N][N][N];
int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
int main(){
    int n,m,start,end, pathcount;//行 列 起始位置 终止位置，步数 //总觉得这个数稍微大一点爆int
    cin>>n>>m>>start>>end>>pathcount;

    for(int i = 0; i<n;i++){
        f[i][0][1]++;
        f[i][m-1][1]++;
    }
    for(int i = 0;i<m;i++){
        f[0][i][1]++;
        f[m-1][i][1]++;
    }
    for(int k = 1;k<=pathcount;k++){
        for(int i = 0;i<n;i++){
            for(int j = 0;j<m;j++){
                for(int u= 0;u<4;u++){
                    int x = i + dx[u], y = j + dy[u];
                    if(x>=0&&x<n&&y>=0&&y<n){
                        f[x][y][k] += f[i][j][k-1];
                    }
                }
            }
        }
    }
    uint64_t ans = 0;
    for(int i = 0;i<=pathcount;i++){
        ans += f[start][end][i];
    }

    cout<<(double)ans/pow(4, pathcount)<<endl;
    return 0;
}