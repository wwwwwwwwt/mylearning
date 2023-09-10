#include <bits/stdc++.h>
using namespace std;
using  ll = long long;
const int N = 1010;
ll f[N];

int main(){
    int n;
    cin>>n;
    f[1] = 1;
    for(int i =2; i <= n; i++){
        f[i] = 1;
        for(int j = 1; j * 2 <= i; j++){
            f[i] += f[j];
        }
        
    }
    
    cout<<f[n]<<endl;
    return 0;
}