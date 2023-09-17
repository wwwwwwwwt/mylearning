/*
 * @Author: zzzzztw
 * @Date: 2023-09-16 17:42:11
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-16 17:51:29
 * @FilePath: /myLearning/算法/洛谷/p1125.cpp
 */
#include <bits/stdc++.h>
using namespace std;
int primes[110];
int cnt;
bool st[110];
void init(){
    for(int i = 2; i <= 110; i++){
        if(!st[i])primes[cnt++] = i;
        for(int j = 0; primes[j] <= 110 / i; j++){
            st[primes[j] * i] = true;
            if(i % primes[j] == 0)break;
        }
    }
}

int main(){
    init();
    string str;
    cin>>str;
    set<int>st;
    for(int i = 0; i < cnt; i++)st.insert(primes[i]);
    unordered_map<int, int>a;
    int mx = -1, mi = 110;
    for(int i = 0; i < str.size(); i++){
        a[str[i] - 'a']++;
    }
    for(auto x : a){
        mx = max(mx, x.second);
        mi = min(mi, x.second);
    }
    if(st.find(mx - mi) == st.end()){
        cout<<"No Answer"<<endl;
        cout<<0<<endl;
        return 0;
    }
    cout<<"Lucky Word"<<endl;
    cout<<mx - mi<<endl;
    return 0;
}