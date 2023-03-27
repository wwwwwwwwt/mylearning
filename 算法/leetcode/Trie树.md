<!--
 * @Author: zzzzztw
 * @Date: 2023-03-24 08:48:44
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-27 16:54:12
 * @FilePath: /cpptest/算法/leetcode/Trie树.md
-->
# Trie树
典型题目：求字符串出现几次， 求最大异或

特别注意：const int N 这个N 为所有字符串的长度总和

## 模板求字符串类型
* acwing 835
```cpp

#include <bits/stdc++.h>
using namespace std;
const int N = 100010;
int son[N][26],cnt[N],idx;

void insert(string &str){
    int p = 0;
    for(int i = 0;i<str.size();i++){
        int u = str[i] - 'a';
        if(!son[p][u])son[p][u] = ++idx;
        p = son[p][u];
    }
    cnt[p]++;
}

int query(string &str){
    int p = 0;
    for(int i = 0;i<str.size();i++){
        int u = str[i] - 'a';
        if(!son[p][u])return 0;
        p = son[p][u];
    }
    return cnt[p];
}

int main(){
    int n;
    cin>>n;
    string str;
    while(n--){
        char op;
        cin>>op;
        cin>>str;
        if(op == 'I')insert(str);
        else cout<<query(str)<<endl;
    }
    return 0;
}

```

## 模板求最大异或值
在给定的 N个整数 A1，A2……AN中选出两个进行 xor（异或）运算，得到的结果最大是多少？
输入格式
第一行输入一个整数 N。

第二行输入 N个整数 A1～AN。

输出格式
输出一个整数表示答案。

数据范围
1≤N≤105,0≤Ai<2^31  

输入样例：
3
1 2 3  
输出样例：
3
```cpp

#include <bits/stdc++.h>
using namespace std;
const int N = 3100010;
const int M = 100010;
int son[N][2], idx;
int a[M];

void insert(int x)
{
    int p = 0;
    for(int i = 30;i>=0;i--){
        int u = x >> i & 1;
        if(!son[p][u]) son[p][u] = ++idx;
        p = son[p][u];
    }
}

long long query(int x){
    int p = 0, res = 0;
    for(int i = 30 ;i>=0;i--){
        int u = x >>i &1;
        if(son[p][!u]){
            res += 1<<i;
            p = son[p][!u];
        }
        else p = son[p][u];
    }
    return res;
}

int main(){
    int n;
    cin>>n;
    for(int i = 0;i<n;i++){
        cin>>a[i];
        insert(a[i]);
    }
    long long res = 0;
    for(int i = 0;i<n;i++){
        res = max(res,query(a[i]));
    }

    cout<<res;
    return 0;
}

```

## 求最大异或值进阶版 最大异或和
* 思路 前缀和 + 前缀树
* 子数组l~r的异或和a[l]^...^a[r] 转化为前缀异或和数组(s[r]^s[l-1])
* 遍历s数组每轮内查询s[i]在合法范围的最大异或和 最后得到结果
* cnt数组存当前编号为p的节点出现的次数 

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 100010*31, M = 100010;
int son[N][2], idx, cnt[N], s[N];

void insert(int x ,int v){
    int p = 0;
    for(int i = 30 ;i>=0;i--){
        int u = x >>i &1;
        if(!son[p][u])son[p][u] = ++idx;
        p = son[p][u];
        cnt[p] += v;
    }
}

int query(int x){
    int p = 0, res = 0;
    for(int i = 30;i >= 0;i-- ){
        int u = x >>i&1;
        if(cnt[son[p][!u]]){
            p = son[p][!u];
            res += 1<<i;
        }
        else p = son[p][u];
    }
    return res;
}

int main(){
    int n,m;
    cin>>n>>m;
    for(int i = 1;i<=n;i++){
        int a;
        cin>>a;
        s[i] = s[i-1]^a;

    }
    insert(s[0], 1);// 空数组也可以是子数组
    int res = 0;
    for(int i = 1;i<=n;i++){
        if(i - m -1 >=0 ){
            insert(s[i-m-1],-1);
        }
        res = max(res, query(s[i]));
        insert(s[i],1);
    }

    cout<<res;

    return 0;

}


```