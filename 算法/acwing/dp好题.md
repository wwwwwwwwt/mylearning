<!--
 * @Author: zzzzztw
 * @Date: 2023-09-18 14:21:05
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-19 14:43:00
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