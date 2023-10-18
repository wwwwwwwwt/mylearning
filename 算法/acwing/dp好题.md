<!--
 * @Author: zzzzztw
 * @Date: 2023-09-18 14:21:05
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-17 13:08:21
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

## 有依赖的背包问题， 树上01背包

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 110;
int n, m;
int f[N][N], v[N], w[N];
vector<int>g[N];
void dfs(int u){
    for(int i = v[u]; i <= m; i++)f[u][i] = w[u]; // 只选当前u为根的节点，初始化
    for(auto c : g[u]){ // 枚举子树节点
        dfs(c);
        for(int j = m; j >= v[u]; j--){ // 01背包倒序枚举总体积，必须大于当前根的体积才能选根节点的子树。
            for(int k = 0; k <= j - v[u]; k++){ // 给子树剩余的空间为当前背包总体积 - 根体积
                f[u][j] = max(f[u][j], f[u][j - k] + f[c][k]);
            }
        }
    }
}
int main(){
    cin>>n>>m;
    int root = 0;
    for(int i = 1; i <= n; i++){
        int p;
        cin>>v[i]>>w[i]>>p;
        if(p == -1)root = i;
        else{
            g[p].push_back(i);
        }
    }
    dfs(root);
    cout<<f[root][m]<<endl;
    return 0;
}
```

## 背包问题求具体方案
* 字典序最小需要倒序枚举物品，没有这个条件的话无所谓

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1010;
int f[N][N], v[N], w[N];
using pii = pair<int, int>;
pii g[N][N];
int main(){
    int n, m;
    cin>>n>>m;
    for(int i = 1; i <= n; i++){
        cin>>v[i]>>w[i];
    }
    for(int i = n; i >= 1; i--){
        for(int j = 0; j <= m; j++){
            f[i][j] = f[i + 1][j];
            g[i][j] = {j, 0};
            if(j >= v[i]){
                if(f[i][j] <= f[i + 1][j - v[i]] + w[i]){
                    f[i][j] = f[i + 1][j - v[i]] + w[i];
                    g[i][j] = {j - v[i], i};
                }
            }
        }
    }
    int j = m, i = 1;
    vector<int>res;
    while(j){
        if(g[i][j].second != 0)res.push_back(g[i][j].second);
        j = g[i][j].first;
        i++;
    }
    for(int i = 0; i < res.size(); i++)cout<<res[i]<<" ";
    return 0;
}

```


## 背包问题求最优解的方案数
* g数组记录当前容量背包最优解方案数。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1010, Mod = 1e9 + 7;
long long f[N], g[N];
int v[N], w[N];

int main(){
    int n,m;
    cin>>n>>m;
    for(int i = 0; i < n; i++)cin>>v[i]>>w[i];
    g[0] = 1;
    long long res = 0;
    for(int i = 0; i < n; i++){
        for(int j = m; j >= v[i]; j--){
            int maxv = max(f[j], f[j - v[i]] + w[i]);
            long long cnt = 0;
            if(maxv == f[j])cnt += g[j];
            if(maxv == f[j - v[i]] + w[i])cnt += g[j - v[i]];
            f[j] = maxv;
            cnt %= Mod;
            g[j] = cnt;
            res = max(res, f[j]);
        }
    }
    long long ans = 0;
    for(int i = 0; i <= m; i++){
        if(f[i] == res){
            ans = (ans + g[i])%Mod;
        }
    }
    cout<<ans<<endl;
    return 0;
}

```

## 能量石， 贪心 + dp
* 贪心思路如耍杂技的牛和国王游戏，排序完按照这个序列做01背包，但由于依赖背包容量（时间）不一定背包越大结果最优，所以一遍做，一边记录最大值，
```cpp
#include <bits/stdc++.h>
using namespace std;
const int  N = 10010;
int f[N];
struct Node{
    int s,e,l;
}a[N];

int main(){
    int t;
    cin>>t;
    for(int i = 1; i <= t; i++){
        memset(f, 0, sizeof f);
        memset(a, 0, sizeof a);
        int n, m = 0;
        cin>>n;
        for(int i = 0; i < n; i++){
            int s, e, l;
            cin>>s>>e>>l;
            a[i] = {s, e, l};
            m += s;
        }
        sort(a, a + n, [&](auto&p, auto&q){
            return p.s * q.l < q.s * p.l;
        });
        int res = 0;
        for(int i = 0; i < n; i++){
            for(int j = m; j >= a[i].s; j--){
                f[j] = max(f[j], f[j - a[i].s] + a[i].e - (j - a[i].s) * a[i].l);
                res = max(res, f[j]);
            }
        }
       cout<<"Case #"<<i<<": "<<res<<endl;
    }
    return 0;
}

```

## 有依赖的分组背包，组内成员比较少---二进制枚举 + 01背包

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 110;
vector<int>g[N];//记录依赖节点的组内节点
vector<int>q; // 记录依赖节点实际下标
int v1[N], v2[N], w1[N], w2[N]; // v1w1为依赖背包体积价值， v2w2为组内子节点体积价值
int f[32010]; // dp
int main(){
    int m,n;
    cin>>m>>n;
    for(int i = 1; i <= n; i++){
        int a, b, p;
        cin>>a>>b>>p;
        if(p == 0){
            q.push_back(i);
            v1[i] = a, w1[i] = b;
        }else{
            g[p].push_back(i);
            v2[i] = a, w2[i] = b;
        }
    }
    // 以上全为建图


    for(int i = 0; i < q.size(); i++){ // 枚举依赖背包节点
        for(int j = m; j >= 0; j--){ // 01背包体积遍历
            int idx = q[i]; //依赖背包下标
            int tv = v1[idx], tw = v1[idx] * w1[idx]; // 本题价值为体积 * 价值参数
            for(int k = 0; k < (1 << g[idx].size()); k++){ // 二进制枚举 组内选哪个
                tv = v1[idx], tw = v1[idx] * w1[idx]; // 每遍历一次就初始化一下
                if(j >= tv)f[j] = max(f[j], f[j - tv] + tw);
                for(int o = 0; o < g[idx].size(); o++){ // 枚举1
                    if((k >> o) & 1){
                        int curidx = g[idx][o]; // 组内背包实际下标
                        tv += v2[curidx], tw += w2[curidx] * v2[curidx]; // 加上
                        if(j >= tv)f[j] = max(f[j], f[j - tv] + tw);
                    }
                }
            }
           
        }
    }
    cout<<f[m]<<endl;
    return 0;
}

```

## CSP-J2019 纪念品

* 做t-1天完全背包，这一天的盈利是-今天价格 + 明天价格， 每一轮背包容量都会改变。

```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
int T, N, M;
int price[110][110];
int f[10010];
void solve() {
    cin>>T>>N>>M;
    int res = 0;
    for(int i = 1; i <= T; i++){
        for(int j = 1; j <= N; j++){
            cin>>price[i][j];
        }
    }
    for(int i = 1; i < T; i++){
        memset(f, 0, sizeof f);
        for(int j = 1; j <= N; j++){
            for(int k = price[i][j]; k <= M; k++){
                f[k] = max(f[k], f[k - price[i][j]] - price[i][j] + price[i + 1][j]);
            }
        }
        M = max(M, f[M] + M);
    }
    cout<<M<<endl;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
```

## 树形dp

1. 所有边都需要被看到

```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
const int N = 1500;
vector<int>g[N];
int f[N][2];
int n;
void dfs(int u, int fa){
    f[u][0] = 0, f[u][1] = 1;
    for(auto c : g[u]){
        if(c != fa){
            dfs(c, u);
            f[u][0] += f[c][1];
            f[u][1] += min(f[c][0], f[c][1]);    
        }
    }
}
int main(){
    while(~scanf("%d",&n)){
        for(int i = 0; i <= n; i++)g[i].clear();
        memset(f, 0x3f, sizeof f);
        for(int i = 0; i < n; i++){
            int a, b, sz;
            scanf("%d:(%d)", &a, &sz);
            while (sz--) {
                cin >> b;
                g[a].push_back(b);
                g[b].push_back(a);
            }
        }
        dfs(1, -1);
        cout<<min(f[1][0], f[1][1])<<endl;
    }
    return 0;
}

```

2. 所有点都需要被看到

```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
const int N = 1510;
vector<int>g[N];
int f[N][3]; // f[i][0]表示被父节点看， f[i][1]表示被自己看， f[i][2]表示被子节点看
int n;
int w[N];
void dfs(int u, int fa){
    f[u][0] = 0, f[u][1] = w[u];
    for(auto c : g[u]) {
        if (c != fa) {
            dfs(c, u);
        }
    }
    for(auto c : g[u]){
        f[u][0] += min(f[c][1], f[c][2]);
        f[u][1] += min(f[c][0], min(f[c][1], f[c][2]));
    }
    for(auto c : g[u]){
        f[u][2] = min(f[u][2], f[u][0] + f[c][1] - min(f[c][1], f[c][2]));
    }
}
bool st[N];
int main(){
    cin>>n;
    for(int i = 0; i < n; i++){
        int a, b, c;
        cin>>a>>w[a]>>c;
        while(c--) {
            cin >> b;
            g[a].push_back(b);
            st[b] = true;
        }
    }
    memset(f, 0x3f, sizeof f);
    int root = -1;
    for(int i = 1; i <= n; i++){
        if(!st[i]){
            root = i;
            dfs(i, -1);
            break;
        }
    }
    cout<<min(f[root][1], f[root][2]);
    return 0;
}


```