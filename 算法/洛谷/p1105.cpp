/*
 * @Author: zzzzztw
 * @Date: 2023-09-16 10:01:07
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-16 10:34:03
 * @FilePath: /myLearning/算法/洛谷/p1105.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 1010;

struct Node{
    int id, h, l, r;
}a[N];
int s[N];
int main(){
    int n;
    cin>>n;
    for(int i = 0; i < n; i++){
        int h, l, r;
        cin>>h>>l>>r;
        a[i] = {i + 1, h, l, r};
    }
    sort(a, a + n, [&](auto&p, auto& q){
        return p.h == q.h ? p.id < q.id : p.h > q.h;
    });
    for(int i = 0; i < n; i++){
        int id = a[i].id;
        s[id - 1] = i;
    }
    for(int i= 0; i < n; i++){
        int left = 0, right = 0;
        for(int  j = 0; j < n; j++){
            if(a[s[i]].l > a[j].l  && a[s[i]].l < a[j].r && a[s[i]].h > a[j].h){
                left = a[j].id;
                break;
            }
        }
        for(int  j = 0; j < n; j++){
            if(a[s[i]].r < a[j].r  && a[s[i]].r > a[j].l && a[s[i]].h > a[j].h){
                right = a[j].id;
                break;
            }
        }
        cout<<left<<" "<<right<<endl;
    }
    return 0;
}