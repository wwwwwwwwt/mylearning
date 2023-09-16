/*
 * @Author: zzzzztw
 * @Date: 2023-09-16 09:00:44
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-16 09:23:10
 * @FilePath: /myLearning/算法/洛谷/p1111.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
using pii = pair<int, int>;
bool st[N];
int dis[N];
vector<pii>g[N];
void dijkstra(){
    priority_queue<pii, vector<pii>, greater<pii>>q;
    memset(dis, 0x3f, sizeof dis);
    dis[0] = 0;
    q.push({dis[0], 0});
    while(q.size()){
        auto t = q.top();
        q.pop();
        if(st[t.second])continue;
        st[t.second] = true;
        for(auto c : g[t.second]){
            if(dis[c.first] > max(dis[t.second], c.second)){
                dis[c.first] = max(dis[t.second], c.second);
                q.push({dis[c.first], c.first});
            }
        }
    }
}
int main(){
    int n, m;
    cin>>n>>m;
    g[0].push_back({1, 0});
    while(m--){
        int x, y, t;
        cin>>x>>y>>t;
        g[x].push_back({y, t});
        g[y].push_back({x, t});
    }
    dijkstra();
    int res = 0;
    for(int i = 1; i <= n; i++)res = max(res, dis[i]);
    cout<<(res == 0x3f3f3f3f? -1 : res )<<endl;
    return 0;

}