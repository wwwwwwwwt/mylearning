#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main(){
    ll res = 0;
    int M;
    cin>>M;
    int l = 0, r = 0;
    while(r < M){
        res += r;
        while(l < r && res > M){
            res -= l;
            l++;
        }
        if(res == M){
            cout<<l<<" "<<r<<endl;
        }
        r++;
    }
    return 0;
}