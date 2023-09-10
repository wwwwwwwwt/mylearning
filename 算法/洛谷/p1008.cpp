/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 17:24:11
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 17:32:46
 * @FilePath: /myLearning/算法/洛谷/p1008.cpp
 */
#include <bits/stdc++.h>
using namespace std;

bool check(int i, int j, int k){
    set<int>st;
    while(i){st.insert(i % 10); i /= 10;}
    while(j){st.insert(j % 10); j /= 10;}
    while(k){st.insert(k % 10); k /= 10;}
    for(int a = 1; a <= 9; a++)if(st.find(a) == st.end())return false;
    return true;
}
int main(){
    vector<vector<int>>res;
    for(int i = 100; 3 * i <= 1000; i++){
        int j = 2 * i, k = 3 * i;
        if(check(i, j, k))res.push_back({i, j, k});
    }
    for(auto x : res){
        cout<<x[0] <<" "<<x[1]<<" "<<x[2]<<endl;
    }
    return 0;
}