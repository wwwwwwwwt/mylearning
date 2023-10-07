/*
 * @Author: zzzzztw
 * @Date: 2023-10-07 12:07:44
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-07 12:15:31
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1795.cpp
 */
#include <bits/stdc++.h>
using namespace std;
unordered_set<int>st;
void init(){
    st.insert(1);
    int i = 1;
    long long cur = 1;
    for(; cur < 1e9 + 10; i++){
        cur = cur + i;
        st.insert(cur);
    }
}
int main(){
    init();
    int n;
    cin>>n;
    int a;
    for(int i = 0; i < n; i++){
        scanf("%d", &a);
        if(st.find(a) == st.end())printf("0\n");
        else printf("1\n");
    }
    return 0;
}