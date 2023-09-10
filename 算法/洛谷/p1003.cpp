#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 1e5 + 10;
struct Node{
    int a, b, c, d;
}d[N];

int main(){
    int n;
    cin>>n;
    for(int i = 0; i < n; i++){
        int a, b , g, k;
        cin>>a>>b>>g>>k;
        d[i] = {a, b, g + a - 1, k + b - 1};
    }
    int res = -1;
    int x, y;
    cin>>x>>y;
    for(int i = n - 1; i >= 0; i--){
        if(d[i].a <= x && d[i].c >=x && d[i].b <= y && d[i].d >= y){
            res = i + 1;
            break;
        }
    }
    cout<<res<<endl;
    return 0;
}