<!--
 * @Author: zzzzztw
 * @Date: 2023-08-27 20:57:03
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-05 16:27:41
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