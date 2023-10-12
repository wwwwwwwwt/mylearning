<!--
 * @Author: zzzzztw
 * @Date: 2023-10-10 15:38:45
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-10 21:11:03
 * @FilePath: /myLearning/算法/leetcode/单调队列优化dp.md
-->
# 单调队列优化dp
## 旅行问题，正反各做一遍单调队列，边界得画图，比较恶心
```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
const int N = 1e6 + 10, mod = 1e9 + 7;
ll a[2*N], w[2*N], s[2*N];
bool st[N];
deque<int>q;
void solve(){
    int n;
    cin>>n;
    for(int i = 1; i <= n; i++)cin>>w[i]>>a[i];
    for(int i = 1; i <= n; i++){
        s[i] = s[i + n] = w[i] - a[i];
    }
    for(int i = 1; i <= 2 * n; i++)s[i] += s[i - 1];
    for(int i = 2*n; i >= 1; i--){
        while(q.size() && q.front() >= i + n)q.pop_front();
        while(q.size() && s[q.back()] >= s[i])q.pop_back();
        q.push_back(i);
        if(i <= n && q.size())if(s[q.front()] - s[i - 1] >= 0)st[i] = true;

    }
    a[0] = a[n];
    for(int i = 1; i <= n; i++)s[i] = s[i + n] = w[i] - a[i - 1];
    for(int i = 1; i <= 2 * n; i++){
        s[i] += s[i + 1];
    }
    while(q.size())q.pop_back();
    for(int i = 1; i <= 2 * n; i++){
        while(q.size() && q.front() < i - n)q.pop_front();
        if(i > n){
            if(q.size() && s[q.front()] <= s[i])st[i - n] = true;
        }
        while(q.size() && s[q.back()] <= s[i])q.pop_back();
        q.push_back(i);
    }
    for(int i = 1; i <= n; i++)if(st[i]){
        cout<<"TAK"<<"\n";
    }else cout<<"NIE"<<"\n";

}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
```

## 连续m个物品，必须选一个，求消费最小

```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
const int N = 2e5 + 10, mod = 1e9 + 7;
int a[N], f[N];
deque<int>q;
void solve(){
    int n, m;
    cin>>n>>m;
    for(int i = 1; i <= n; i++)cin>>a[i];
    for(int i = 1; i <= n; i++){
        while(q.size() && i - q.front() > m)q.pop_front(); // 这一行大于m，i中必须在前m个中选一个，才能保证前m个不相连， 把往前数第m+1个数弹出去，如果包括的话可能中间m个相连了。(不是等于号，等于号就相当于中间只隔了m-1个不相连)
        if(i > m && q.size())f[i] = f[q.front()] + a[i];
        else f[i] = a[i];
        while(q.size() && a[q.back()] >= a[i])q.pop_back();
        q.push_back(i);
    }
    int res = 1e9;
    for(int i = n; i > n - m; i--)res = min(res, f[i]);
    cout<<res<<endl;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}

```

## 二维滑窗

```cpp
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
const int N = 1050, mod = 1e9 + 7;
int a[N][N], row_max[N][N], row_min[N][N];
int e[N], b[N], c[N];
int n, m, k;


void get_max(int x[], int y[], int tot){
    deque<int>q;
    for(int i = 1; i <= tot; i++){
        while(q.size() && i - q.front() >= k)q.pop_front();
        while(q.size() && x[q.back()] <= x[i])q.pop_back();
        q.push_back(i);
        if(i >= k){
            if(q.size())y[i] = x[q.front()];
            else y[i] = x[i];
        }
    }
}
void get_min(int x[], int y[], int tot){
    deque<int>q;
    for(int i = 1; i <= tot; i++){
        while(q.size() && i - q.front() >= k)q.pop_front();
        while(q.size() && x[q.back()] >= x[i])q.pop_back();
        q.push_back(i);
        if(i >= k){
            if(q.size())y[i] = x[q.front()];
            else y[i] = x[i];
        }
    }
}
void solve(){
    cin>>n>>m>>k;
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++)cin>>a[i][j];
    }

    // 枚举每一行，对每一行做滑窗
    for(int i = 1; i <= n; i++){
        get_min(a[i], row_min[i], m);
        get_max(a[i], row_max[i], m);
    }
    int res = INT_MAX;

    // 枚举每一列
    for(int i = k; i <= m; i++){
        // 枚举每一行，e[j]保存这一列竖着的值提取出来，在行上，竖着做滑窗
        for(int j = 1; j <= n; j++)e[j] = row_min[j][i];
        get_min(e, b, n);
        for(int j = 1; j <= n; j++)e[j] = row_max[j][i];
        get_max(e, c, n);
        for(int j = k; j <= n; j++)res = min(res, c[j] - b[j]);
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