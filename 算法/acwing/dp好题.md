<!--
 * @Author: zzzzztw
 * @Date: 2023-09-18 14:21:05
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-27 22:01:32
 * @FilePath: /myLearning/算法/acwing/dp好题.md
-->

## 方格取数
* 两个人一起走，就不能两维dp了，需要三维，k步，第一个人向下走i1步，第二个人向下走i2步
```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 15;
int a[N][N];
int f[35][15][15];

int main(){
    int n;
    cin>>n;
    int i, j, w;
    while(cin>>i>>j>>w, i||j||w)a[i][j] = w;
    for(int k = 2; k <= 2 * n; k++){
        for(int i1 = 1; i1 <= k; i1++){
            for(int i2 = 1; i2 <= k; i2++){
                int j1 = k - i1, j2 = k - i2;
                if(j1 >= 1 && j1<= n && j2 >= 1 && j2 <= n){
                    int t = a[i1][j1];
                    if(i1 != i2)t+= a[i2][j2];
                    f[k][i1][i2] = max(f[k][i1][i2], f[k - 1][i1 - 1][i2] + t);
                    f[k][i1][i2] = max(f[k][i1][i2], f[k - 1][i1][i2 - 1] + t);
                    f[k][i1][i2] = max(f[k][i1][i2], f[k - 1][i1][i2] + t);
                    f[k][i1][i2] = max(f[k][i1][i2], f[k - 1][i1 - 1][i2 - 1] + t);
                }
            }
        }
    }
    cout<<f[2 * n][n][n]<<endl;
    return 0;
}
```

## 最长公共上升子序列
* f[i][j]表示前i，j个下标中，以b[j]为结尾的最长公共上升子序列。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 3010;
int f[N][N];
int n;
int a[N],b[N];
int res;
int main(){
    cin>>n;
    for(int i = 1; i <= n; i++)cin>>a[i];
    for(int i = 1; i <= n; i++)cin>>b[i];
    for(int i = 1; i <= n; i++){
        int maxv = 1;
        for(int j = 1; j <=n; j++){
            f[i][j] = f[i - 1][j];
            if(a[i] == b[j])f[i][j] = max(maxv, f[i][j]);
            if(b[j] < a[i])maxv = max(maxv, f[i - 1][j] + 1);
        }
    }
    for(int i = 1; i <= n; i++)res = max(res, f[n][i]);
    cout<<res<<endl;
    return 0;
}

```

## 拦截导弹
* 贪心思路，如果能找到接到当前末尾的最后面就直接放在后面，否则新开一个。
```cpp
#include <bits/stdc++.h>
using namespace std;

const int N = 1010;
int a[N];
int f[N];
vector<int>g;
int cnt, res;
int main(){
    int n = 0;
    int x;
    while(cin>>x)a[n++] = x;
    for(int i = 0; i < n; i++){
        f[i] = 1;
        for(int j = 0; j < i; j++){
            if(a[i]<= a[j])f[i] = max(f[i], f[j] + 1);
        }
        res = max(res, f[i]);
        int p = lower_bound(g.begin(), g.end(), a[i]) - g.begin();
        if(p == cnt){
            g.push_back(a[i]);
            cnt++;
        }else g[p] = a[i];
    }
    cout<<res<<endl;
    cout<<cnt<<endl;
    return 0;
}
```
## 导弹防御系统
* 放爆搜回溯类型更合适，但是思想用到了导弹防御的最长上升子序列的思想。
```cpp
#include <bits/stdc++.h>
using namespace std;
int n;
const int N = 55;
int a[N];
int up[N], down[N];
int res;
void dfs(int idx, int u, int d){
    if(u + d >= res)return;
    if(idx == n){
        res = min(res, u + d);
        return;
    }
    int i;
    for(i = 0; i < u; i++){
        if(up[i] < a[idx]){
            break;
        }
    }
    
    int k = up[i];
    up[i] = a[idx];
    dfs(idx + 1, u + (i == u ? 1:0), d);
    up[i] = k;
    
    for(i = 0; i < d; i++){
        if(down[i] > a[idx]){
            break;
        }
    }
    k = down[i];
    down[i] = a[idx];
    dfs(idx + 1, u, d + (i == d ? 1 : 0));
    down[i] = k;
}
int main(){
    while(cin>>n,n){
        for(int i = 0; i < n; i++)cin>>a[i];
        res = INT_MAX;
        dfs(0, 0, 0);
        cout<<res<<endl;
    }
    return 0;
}
```

## 安排邮筒 lc1478
* 抽象成 f[i][j]表示分成i份，前j个房子 最小距离， 需要枚举， 前一个点进行dp 类似整理书架那题。
* 预处理区间中距离，主要这个区间内，不管邮箱在哪里总距离都不变，总距离由递推得出。
```cpp
class Solution {
public:
    static const int N = 110;
    int f[N][N], dis[N][N];
    int minDistance(vector<int>& houses, int k) {
        sort(houses.begin(), houses.end());
        for(int i = 0; i < houses.size(); i++){
            for(int j = i; j >= 0; j--){
                if(i == j)dis[i][i] = 0;
                if(i == j + 1)dis[i][j] = houses[i] - houses[j];
                else if(i > j + 1)dis[i][j] = dis[i-1][j+1] + houses[i] - houses[j]; 
            }
        }
        memset(f, 0x3f, sizeof f);
        
        for(int i = 0; i < houses.size(); i++)f[1][i] = dis[i][0];
        for(int i = 1; i <= k; i++){
            for(int j = i - 1; j < houses.size(); j++){
                if(j == 0)f[i][j] = min(f[i][j], dis[j][0]);
                else{
                    for(int p = j - 1; p >= i - 2; p--){
                        if(p < 0)continue;
                        f[i][j] = min(f[i][j], f[i - 1][p] + dis[j][p + 1]);
                    }
                }
            }
        }
        return f[k][houses.size() - 1];
    }
};
```

## 单调队列优化多重背包
* 01 / 完全 / 混合背包都可以转换为多重背包来做吗， 完全背包相当于给了 容量 /物品体积 个物品
```cpp
#pragma G++ optimize(2)
#include <bits/stdc++.h>
using namespace std;
long long f[20010], g[20010];
int q[20010];

int main(){
    int n,m;
    cin>>n>>m;
    for(int i = 0; i < n; i++){
        int v, w, s; //每组的体积，价值，数量
        cin>>v>>w>>s;
        memcpy(g, f, sizeof f); // 滚动数组优化一维空间
        for(int j = 0; j < v; j++){ // 枚举 模 体积后的余数
            int hh = 0, tt = -1; // 单调队列，队列窗口大小是当前元素的体积 * 数量，内部元素是在加小于s个当前物品的条件下，能转移到当前k体积条件下的体积值，队首为能得到的最大值 （需要加上体积差 / 物品体积 * 物品价值）
            for(int k = j; k <= m; k+=v){ // 枚举体积
                while(hh <= tt && q[hh] < k - s * v)++hh; // 如果发现队头元素 小于当前体积减去所有当前物品个数*体积，代表着当前单调队列中的最大值不能转移到当前体积内 那么就将队头元素pop掉
                while(hh <= tt && g[q[tt]] <= g[k] - (k - q[tt])/v * w)--tt;
                q[++tt] = k;
                f[k] = max(f[k], g[q[hh]] + (k - q[hh]) / v * w);
            } 
        }
    }
    cout<<f[m]<<endl;
    return 0;
}

```

## 填满背包容量（可以超过）的最低费用
* 每一位都可以和0取个max，表示可以超过当前容量。a，b分别为两维的容量，w为费用
```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1010;
int a[N], b[N], w[N];
int f[N][N];
int main(){
    int n, m;
    cin>>n>>m;
    int t;
    cin>>t;
    memset(f, 0x3f, sizeof f);
    f[0][0] = 0;
    for(int i = 0; i < t; i++)cin>>a[i]>>b[i]>>w[i];
    for(int i = 0; i < t; i++){
        for(int j = n; j >= 0; j--){
            for(int k = m; k >= 0; k--){
                f[j][k] = min(f[j][k], f[max(j - a[i], 0)][max(k - b[i], 0)] + w[i]);
            }
        }
    }
    cout<<f[n][m]<<endl;
    return 0;
}

```