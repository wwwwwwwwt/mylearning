<!--
 * @Author: zzzzztw
 * @Date: 2023-08-27 20:57:03
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-24 19:54:34
 * @FilePath: /myLearning/算法/leetcode/lca相关,.md
-->
# lca 树上倍增

1. 最简单模板题 1483
* 思路：动态规划倍增思想
```cpp
class TreeAncestor {
public:
    int pa[50050][35];
    TreeAncestor(int n, vector<int>& parent) {
        memset(pa, -1, sizeof pa);
        for(int i = 0; i < parent.size(); i++){
            pa[i][0] = parent[i];
        }
        for(int j = 1; j <= 32; j++){
            for(int i = 0; i < parent.size(); i++){
                if(pa[i][j-1] == -1)continue; 
                pa[i][j] = pa[pa[i][j-1]][j-1];
            }
        }
    }
    
    int getKthAncestor(int node, int k) {
        for(int j = 0; j < 32; j++){
            if((k >> j) & 1){
                node = pa[node][j];
                if(node < 0)break;
            }
        }
        return node;
    }
};


```

1. 在传球游戏中最大化函数值 2836


* 思路：pa记录i节点跳j次后的父节点，f记录i节点跳j次后的路径值，但不包括跳j次后终点值，所以最后得加上
```cpp
class Solution {
public:
    long long getMaxFunctionValue(vector<int>& receiver, long long k) {
        int pa[100010][35];
        long long f[100010][35];
        for(int i = 0; i < receiver.size(); i++){
            pa[i][0] = receiver[i];
            f[i][0] = i;
        }
        for(int j = 1; j < 35; j++){
            for(int i = 0; i < receiver.size(); i++){
                pa[i][j] = pa[pa[i][j-1]][j-1];
                f[i][j] = f[i][j - 1] + f[pa[i][j-1]][j-1];
            }
        }
        long long ans = 0;
        for(int i = 0; i < receiver.size(); i++){
            int pos = i;
            long long cur = 0;
            for(int j = 0; j < 35; j++){
                if((k >> j) & 1){
                    cur += f[pos][j];
                    pos = pa[pos][j];
                }
            }
            ans = max(ans, cur + pos);
        }
        return ans;
    }
};

```

2. 边权重均等查询  2846

```cpp
class Solution {
public:
    static const int N = 10010;
    using pii = pair<int, int>;
    vector<pii>g[N];
    int cnt[N][16][27];
    int deepth[N];
    int pa[N][16];
    void dfs(int x, int fa){
        pa[x][0] = fa;
        for(auto c : g[x]){
            if(c.first != fa){
                cnt[c.first][0][c.second] = 1;
                deepth[c.first] = deepth[x] + 1;
                dfs(c.first, x);
            }
        }
    }
    vector<int> minOperationsQueries(int n, vector<vector<int>>& edges, vector<vector<int>>& queries) {
       
        //建图并预处理父节点和边权，边权指向父节点
        for(auto x : edges){
            int a = x[0], b = x[1], c = x[2];
            g[a].push_back({b, c});
            g[b].push_back({a, c});
        }
        memset(pa, -1, sizeof pa);
        dfs(0, -1);
        
        // lca模板，并预处理i节点向上跳2的j次方，所拥有的边权，（包括最后一个节点到其父节点的边权）
        for(int j = 1; j < 15; j++){
            for(int i = 0; i < n; i++){
                if(pa[i][j-1] != -1){
                    pa[i][j] = pa[pa[i][j - 1]][j - 1];
                    for(int k = 0; k <= 26; k++){
                        cnt[i][j][k] = cnt[i][j - 1][k] + cnt[pa[i][j-1]][j-1][k];
                    }
                }
            }
        }
        vector<int>res;
        for(auto x : queries){
            int a = x[0], b = x[1];
            if(a == b){
                res.push_back(0);
                continue;
            }
            // 公式是深度 - 2 * lca深度 就是两个点之间的边数， 再减去出现最多的数字就是答案
            int totaldeepth = deepth[a] + deepth[b];
            int curcnt[27] = {0};
            if(deepth[a] > deepth[b])swap(a, b);

            // 把深度分解成二进制，类似查询第k的祖先，跳到一样的深度，边跳边累加出现次数
            int dif = deepth[b] - deepth[a];
            for(int i = 0; i < 15; i++){
                if((dif >> i) & 1){
                    for(int k = 0; k <= 26; k++){
                        curcnt[k] += cnt[b][i][k];
                    }
                    b = pa[b][i];
                }
            }

            // 一起向上跳，从大到小枚举，能跳多远。
            if(a != b){
                for(int i = 15; i >= 0; i--){
                    int la = pa[a][i], lb = pa[b][i];
                    if(la == -1)continue;
                    if(la == lb)continue;
                    if(la != lb){
                        for(int k = 0; k <= 26; k++){
                            curcnt[k] += cnt[a][i][k] + cnt[b][i][k];
                        }
                        a = la, b = lb;
                    }
                }
                for(int i = 0; i <= 26; i++){
                    curcnt[i] += cnt[a][0][i] + cnt[b][0][i];
                }
            }

            // 看看a b是否是一个点，如果是的话，公共祖先就是a，否则即为a的父节点或b的父节点。
            int lca = a == b ? a : pa[a][0];
            int diff = totaldeepth - 2 * deepth[lca];
            int mac = 0;
            for(int i = 0; i <= 26; i++)mac = max(mac, curcnt[i]);
            res.push_back(diff - mac);
        }
        return res;
    }
};
```


# 3. 给出 n 个点的一棵树，多次询问两点之间的最短距离。求树两点之间距离的模板题
* 最短距离相当于各自到他俩之间的公共祖先的路径距离
```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
const int N = 40010;
vector<pii>g[N];
ll fa[N][20], depth[N];
ll dis[N][20];
int n, m;

// 建树，建立的过程中预处理出来与父节点的距离dis数组和父节点fa数组和深度depth数组
void dfs(int u, int f, int d){
    fa[u][0] = f;
    dis[u][0] = d;
    for(auto c : g[u]){
        if(c.first != f){
            depth[c.first] = depth[u] + 1;
            dfs(c.first, u, c.second);
        }
    }
}
// 预处理，外层循环2的i次幂，内层为点的编号，倍增处理出来与倍增路径上的点的距离和编号
void lcapre(){
    for(int i = 1; i <= 17; i++){
        for(int x = 1; x <= n; x++){
            if(fa[x][i - 1] != 1){
                fa[x][i] = fa[fa[x][i - 1]][i - 1];
                dis[x][i] = dis[x][i - 1] + dis[fa[x][i - 1]][i - 1];
            }
        }
    }
}

// lca跳跃过程
int lca(int a, int b){
    if(a == b)return 0;//同一个点直接返回
    int res = 0;

    // 找出来两点在深度的差值，深的向上跳，直到两个点在同一层，过程中记录距离。
    if(depth[a] < depth[b])swap(a, b);
    int dif = depth[a] - depth[b];
    for(int i = 0; i <= 17; i++){
        if((dif >> i) & 1){
            res += dis[a][i];
            a = fa[a][i];
        }
    }
    // 这一步很关键，如果同一层了，且a等于b点，说明这俩点有一个是对方的公共祖先，在路径上，此时直接返回这个路径距离即可。
    if(a == b)return res;

    // 开始倍增的跳，知道他俩一起跳到公共祖先的下方。
    // 如果la为-1了说明跳过了，跳到最上面的点了，continue
    // la = lb了，说明跳到了或者跳过了，continue
    // la != lb，开始向上跳，记录答案
    for(int i = 17; i >= 0; i--){
        int la = fa[a][i], lb = fa[b][i];
        if(la == -1)continue;
        if(la == lb)continue;
        if(la != lb){
            res += dis[a][i] + dis[b][i];
            a = fa[a][i];
            b = fa[b][i];
        }
    }
    //此时两个点在公共祖先下面，需要加上到公共祖先的距离，返回即可。
    res += dis[a][0] + dis[b][0];
    return res;
}

void solve() {
    cin>>n >> m;
    for(int i = 0; i < n - 1; i++){
        int a, b, c;
        cin>>a>>b>>c;
        g[a].push_back({b,c});
        g[b].push_back({a,c});
    }
    dfs(1, -1, 0);
    lcapre();
    for(int i = 0; i < m; i++){
        int a, b;
        cin>>a>>b;
        cout<<lca(a, b)<<endl;
    }
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
```

## 求次小生成树，lca做法
* 需要在倍增的时候，记录每段中的最小值和次小值
```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
const int N = 100010;
struct Node{
    int u, v, w;
    bool has;
    Node(int a, int b, int c):u(a),v(b), w(c),has(false){};
};
vector<Node>g;
vector<pii>e[N];
ll fa[N][20], depth[N];
ll dis[N][20];
ll dis2[N][20];
int p[N];
bool st[N];
int find(int x){
    if(x != p[x]){
        p[x] = find(p[x]);
    }
    return p[x];
}
int n, m;
void dfs(int u, int f, int d){
    fa[u][0] = f;
    dis[u][0] = d;
    dis2[u][0] = LLONG_MIN;
    st[u] = true;
    for(auto c : e[u]){
        if(c.first != f && !st[c.first]){
            depth[c.first] = depth[u] + 1;
            dfs(c.first, u, c.second);
        }
    }
}

void lcapre(){
    for(int i = 1; i <= 17; i++){
        for(int x = 1; x <= n; x++){
            if(fa[x][i - 1] != 1){
                fa[x][i] = fa[fa[x][i - 1]][i - 1];
                vector<ll>t;
                t.push_back(dis[x][i - 1]);
                t.push_back(dis2[x][i - 1]);
                t.push_back(dis[fa[x][i - 1]][i - 1]);
                t.push_back(dis2[fa[x][i - 1]][i - 1]);
                ll d1 = LLONG_MIN, d2 = LLONG_MIN;
                for(int i = 0; i < t.size(); i++){
                    if(t[i] > d1){
                        d2 = d1;
                        d1 = t[i];
                    }else if(t[i] < d1 && t[i] > d2)d2 = t[i];
                }
                dis[x][i] = d1;
                dis2[x][i] = d2;
            }
        }
    }
}

int lca(int a, int b, int w){
    vector<ll>t;
    if(depth[a] < depth[b])swap(a, b);
    int dif = depth[a] - depth[b];
    for(int i = 0; i <= 17; i++){
        if((dif >> i) & 1){
            t.push_back(dis[a][i]);
            t.push_back(dis2[a][i]);
            a = fa[a][i];
        }
    }
    if(a != b){
        for(int i = 17; i >= 0; i--){
            int la = fa[a][i], lb = fa[b][i];
            if(la == -1)continue;
            if(la == lb)continue;
            if(la != lb){
                t.push_back(dis[a][i]);
                t.push_back(dis2[a][i]);
                t.push_back(dis[b][i]);
                t.push_back(dis2[b][i]);
                a = fa[a][i];
                b = fa[b][i];
            }
        } 
        t.push_back(dis[a][0]);
        t.push_back(dis[b][0]);
    }
    ll d1 = LLONG_MIN, d2 = LLONG_MIN;
    for(int i = 0; i < t.size(); i++){
        if(t[i] > d1){
            d2 = d1;
            d1 = t[i];
        }else if(t[i] < d1 && t[i] > d2)d2 = t[i];
    }
    if(w > d1)return  w - d1;
    if(w > d2)return w - d2;
    return 1e18;
}

void solve() {
    cin>>n>>m;
    for(int i = 1; i <= n; i++)p[i] = i;
    for(int i = 0; i < m; i++){
        int a, b, c;
        cin>>a>>b>>c;
        if(a == b)continue;
        g.push_back({a, b, c});
    }
    sort(g.begin(), g.end(), [&](auto&a, auto&b){
        return a.w < b.w;
    });
    ll sum = 0;
    for(auto &c : g){
        int la = find(c.u), lb = find(c.v), w = c.w;
        if(la != lb){
            sum += w;
            p[lb] = la;
            c.has = true;
            e[c.u].push_back({c.v, w});
            e[c.v].push_back({c.u, w});
        }
    }
    dfs(1, -1, 0);
    lcapre();
    ll res = LLONG_MAX;
    for(auto c : g){
        if(!c.has){
            int u = c.u, v = c.v, w = c.w;
            res = min(res, sum + lca(u, v, w));
        }
    }
    cout<<res<<endl;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}

```