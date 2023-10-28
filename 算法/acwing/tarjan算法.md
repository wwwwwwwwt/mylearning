# tarjan 有向图求强连通分量 （缩点）

## 模板， 缩点后有sc个点，每个点是scc[i]
```cpp
vector<int>g[N];
int dfn[N], low[N];
int sz[N];
int scc[N];
bool inst[N];
int timestamp, sc;
int d[N];
stack<int>st;
void tarjan(int u){
    dfn[u] = low[u] = ++timestamp;
    st.push(u);
    inst[u] = true;
    for(auto c : g[u]){
        if(!dfn[c]){
            tarjan(c);
            low[u] = min(low[u], low[c]);
        }
        else if(inst[c]){
            low[u] = min(low[u], dfn[c]);
        }
    }
    if(dfn[u] == low[u]){
        ++sc;
        while(st.top() != u){
            auto t = st.top();
            st.pop();
            inst[t] = false;
            scc[t] = sc;
            sz[sc]++;
        }
        auto t = st.top();
        scc[t] = sc;
        sz[sc]++;
        inst[t] = false;
        st.pop();
    }
}


.....

// 做完tarjan，根据原数组的情况，重新建图或记录一些出度入度信息
// sc的逆序即为缩点后的拓扑序，最大sc是一开始的起点。
  for(int i = 1; i <= n; i++){
        for(auto c : g[i]){
            if(scc[i] != scc[c]){
                //重新建图
                //建图时记得判重 set<pii>s;
                //记录出度入度
            }
        }
    }
```

## 受欢迎的牛
* 求只有一个没有出边的且只有一个环的强连通分量

```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
const int N = 10010;
int n, m;
vector<int>g[N];
int dfn[N], low[N];
int sz[N];
int scc[N];
bool inst[N];
int timestamp, sc;
int d[N];
stack<int>st;
void tarjan(int u){
    dfn[u] = low[u] = ++timestamp;
    st.push(u);
    inst[u] = true;
    for(auto c : g[u]){
        if(!dfn[c]){
            tarjan(c);
            low[u] = min(low[u], low[c]);
        }
        else if(inst[c]){
            low[u] = min(low[u], dfn[c]);
        }
    }
    if(dfn[u] == low[u]){
        ++sc;
        while(st.top() != u){
            auto t = st.top();
            st.pop();
            inst[t] = false;
            scc[t] = sc;
            sz[sc]++;
        }
        auto t = st.top();
        scc[t] = sc;
        sz[sc]++;
        inst[t] = false;
        st.pop();
    }
}
void solve() {
    cin>>n>>m;
    for(int i = 0; i < m; i++){
        int a, b;
        cin>>a>>b;
        g[a].push_back(b);
    }
    for(int i = 1; i <= n; i++){
        if(!dfn[i]){
            tarjan(i);
        }
    }
    for(int i = 1; i <= n; i++){
        for(auto c : g[i]){
            if(scc[i] != scc[c]){
                d[scc[i]]++;
            }
        }
    }
    int res = 0;
    for(int i = 1; i <= sc; i++){
        if(d[i] == 0 && res == 0)res += sz[i];
        else if(d[i] == 0 && res != 0){
            res = 0;
            break;
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

## 学校网络
* 利用tarjan缩点，记录出度和入度来求起点和终点数量，若缩点完只有一个点则放一个物品就能传递给所有节点，多个起点就得放多次。
* 最少连max（起点数量，终点数量）才能让一个物品传递所有点，形象理解 最少每个起点得放一个，终点得由上游点弄过来，所以终点必须也得连上。详细证明看博客
```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
const int N = 10010;
int n, m;
vector<int>g[N];
int dfn[N], low[N];
int scc[N];
int sz[N];
int timestamp, sc;
stack<int>st;
bool inst[N];
int d[N], o[N];
void tarjan(int u){
    dfn[u] = low[u] = ++timestamp;
    st.push(u);
    inst[u] = true;
    for(auto c : g[u]){
        if(!dfn[c]){
            tarjan(c);
            low[u] = min(low[u], low[c]);
        }
        else if(inst[c]){
            low[u] = min(low[u], dfn[c]);
        }
    }
    if(dfn[u] == low[u]){
        ++sc;
        while (st.top() != u){
            auto t = st.top();
            st.pop();
            scc[t] = sc;
            sz[sc]++;
            inst[t] = false;
        }
        auto t = st.top();
        st.pop();
        inst[t] = false;
        scc[t] = sc;
        sz[sc]++;
    }
}
void solve() {
    cin>>n;
    for(int i = 1; i <= n; i++){
        int a;
        while(cin>>a, a){
            g[i].push_back(a);
        }
    }
    for(int i = 1; i <= n; i++){
        if(!dfn[i])tarjan(i);
    }
    for(int i = 1; i <= n; i++){
        for(auto c : g[i]){
            if(scc[i] != scc[c]){
                d[scc[i]]++;
                o[scc[c]]++;
            }
        }
    }
    if(sc == 1){
        cout<<1<<endl;
        cout<<0<<endl;
        return;
    }
    int p = 0, q = 0;
    for(int i = 1; i <= sc; i++){
        if(d[i] == 0)q++;
        if(o[i] == 0)p++;
    }
    cout<<p<<endl;
    cout<<max(p, q)<<endl;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}

```

## 最大半连通子图 缩点 + 拓扑排序 + 记忆化搜索dp，求缩点后权值最大的一条边及其方案数

```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
const int N = 100010;
int n, m, MOD;
vector<int>g[N];
int dfn[N], low[N];
bool inst[N];
stack<int>st;
int timestamp, sc;
int scc[N];
int sz[N];
int d[N];
vector<int>e[N];
ll f[N], t[N];
ll curmax, tmax;
void tarjan(int u){
    dfn[u] = low[u] = ++timestamp;
    st.push(u);
    inst[u] = true;
    for(auto c : g[u]){
        if(!dfn[c]){
            tarjan(c);
            low[u] = min(low[u], low[c]);
        }else if(inst[c]){
            low[u] = min(low[u], dfn[c]);
        }
    }
    if(dfn[u] == low[u]){
        ++sc;
        while(st.top() != u){
            auto t = st.top();
            st.pop();
            inst[t] = false;
            scc[t] = sc;
            sz[scc[t]]++;
        }
        auto t = st.top();
        st.pop();
        inst[t] = false;
        scc[t] = sc;
        sz[scc[t]]++;
    }
}
void dfs(int u, int fa){
    if(f[u] != 0)return;
    f[u] = sz[u], t[u] = 1;
    curmax = max(curmax, f[u]);
    for(auto c : e[u]){
        if(c != fa){
            dfs(c, u);
            if(f[u] < sz[u] + f[c]){
                f[u] = sz[u] + f[c];
                t[u] = t[c];
                curmax = max(curmax, f[u]);
            }else if(f[u] == sz[u] + f[c]){
                t[u] = (t[u] + t[c]) % MOD;
            }
        }
    }
}
void solve() {
    cin>>n>>m>>MOD;
    set<pii>s;
    for(int i = 0; i < m; i++){
        int a, b;
        cin>>a>>b;
        g[a].push_back(b);
    }
    for(int i = 1; i <= n; i++){
        if(!dfn[i])tarjan(i); // 缩点
    }
    s.clear();
    // 记录入度，从入度为0为起点后序dp
    for(int i = 1; i <= n; i++){
        for(auto c : g[i]){
            if(scc[i] != scc[c] && s.find({scc[i], scc[c]}) == s.end()){
                d[scc[c]]++;
                e[scc[i]].push_back(scc[c]);
                s.insert({scc[i], scc[c]});
            }
        }
    }
    for(int i = 1; i <= sc; i++){
        if(d[i] == 0)dfs(i, -1);
    }
    for(int i = 1; i <= sc; i++){
        if(f[i] == curmax)tmax = (tmax + t[i])%MOD;
    }
    cout<<curmax<<endl;
    cout<<tmax<<endl;

}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
```

# 双连通分量，求桥
* 注意两个点需要特判掉。

## 至少加多少条边才能使这个无向图成为完全的双连通图
```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
const int N = 100010;
int n, m;
vector<int>g[N];
vector<int>bridge[N];
int dfn[N], low[N];
int timestamp, dc;
int dcc[N];
stack<int>st;
int d[N];
void tarjan(int u, int fa){
    dfn[u] = low[u] = ++timestamp;
    st.push(u);
    for(auto c : g[u]){
        if(!dfn[c]){
            tarjan(c, u);
            low[u] = min(low[u], low[c]);
            if(dfn[u] < low[c]){
                bridge[u].push_back(c);
                bridge[c].push_back(u);
            }
        }else if(dfn[c] < dfn[u] && c != fa){
            low[u] = min(low[u], dfn[c]);
        }
    }
    if(dfn[u] == low[u]){
        ++dc;
        while(st.top() != u){
            auto t = st.top();
            st.pop();
            dcc[t] = dc;
        }
        auto t = st.top();
        st.pop();
        dcc[t] = dc;
    }
}
void solve() {
    cin>>n>>m;
    set<pii>s;
    for(int i = 1; i <= m; i++){
        int a, b;
        cin>>a>>b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    // 特判
    if(n == 2){
        cout<<0<<endl;
        return;
    }
    tarjan(1, -1);
    // 根据桥划分出度
    for(int i = 1; i <= n; i++){
        d[dcc[i]] += bridge[i].size();
    }
    int res = 0;
    // 出度为1的就是叶子节点
    for(int i = 1; i <= dc; i++)if(d[i] == 1)res++;
    cout<<(res + 1) / 2;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}

```

## 求一个无向图去掉一个点，最多把图分几块

```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
const int N = 100010;
int n, m;
vector<int>g[N];
int dfn[N], low[N];
int timestamp;
int res;
void tarjan(int u, int root){
    int cnt = 0;
    dfn[u] = low[u] = ++timestamp;
    for(auto c : g[u]){
        if(!dfn[c]){
            tarjan(c, root);
            low[u] = min(low[u], low[c]);
            if(dfn[u] <= low[c])cnt++;
        }else low[u] = min(low[u], dfn[c]);
    }
    if(u != root)cnt++;
    res = max(res, cnt);
}
void solve() {
    while (cin>>n>>m, n || m){
        memset(dfn, 0, sizeof dfn);
        memset(low, 0, sizeof low);
        timestamp = 0, res = 0;
        for(int i = 0; i <= n; i++)g[i].clear();
        for(int i = 0; i < m; i++){
            int a, b;
            cin>>a>>b;
            g[a].push_back(b);
            g[b].push_back(a);
        }
        int cnt = 0;
        for(int i = 0; i < n; i++){
            if(!dfn[i]){
                cnt++;
                tarjan(i, i);
            }
        }
        cout<<(cnt + res - 1)<<endl;
    }
    return;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}

```