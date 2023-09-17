/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 18:31:03
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 19:13:24
 * @FilePath: /myLearning/算法/洛谷/p1036.cpp
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
bool check(ll x){
    if(x < 2)return false;
    for(int i = 2; i <= x / i; i++){
        if(x % i == 0)return false;
    }
    return true;
}
vector<int>a;
int n, k;
ll res;
bool st[25];
ll  cur = 0;
void dfs(vector<int>&a, int idx, int cnt){
   
    if(cnt == k && check(cur)){
        res++;
        return;
    }
    if(cnt > k)return;
    for(int i = idx; i < a.size(); i++){
        st[i] = true;
        cur += a[i];
        dfs(a, i + 1, cnt + 1);
        cur -= a[i];
        st[i] = false;
    }
}

int main(){

    cin>>n>>k;
    for(int i = 0; i < n; i++){
        int x;
        cin>>x;
        a.push_back(x);
    }
    dfs(a, 0, 0);
    cout<<res<<endl;
    return 0;
}