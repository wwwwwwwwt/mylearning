/*
 * @Author: zzzzztw
 * @Date: 2023-03-22 09:37:38
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-22 11:14:29
 * @FilePath: /cpptest/算法/leetcode/test.cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 2510;
long long  a[N],f[N];



long long maxTaxiEarnings(int n, vector<vector<int>>& rides) {
    vector<vector<long long>>a;
    for(int i = 0;i<rides.size(); i++){
        a.push_back({rides[i][0],rides[i][1], rides[i][1] - rides[i][0] + rides[i][2]});
    }

    sort(a.begin(),a.end(),[](auto &a1, auto &b1){
        return a1[1] < b1[1];
    });

    f[0] = a[0][2];

    for(int i = 1;i<a.size();i++){
        int l = 0, r = i - 1;
        while(l<r){
            int mid = l + r + 1>>1;
            if(a[mid][1] <= a[i][0]) l = mid;
            else r = mid - 1;
        }
        if(a[l][1] <= a[i][0]){
            f[i] = max(f[i-1], f[l] + a[i][2]);
        }
        else f[i] = max(f[i-1], a[i][2]);
    }

    return f[rides.size()-1];
};


int main(){
    int n;
    cin>>n;
    for(int i = 0;i<n;i++)cin>>a[i];

    int len = 0;

    for(int i = 0;i<n;i++){
        int l = 0, r = len;
        while(l<r){
            int mid = l + r + 1 >>1;
            if(f[mid] < a[i])l = mid;
            else r = mid - 1;
        }
        len = max(len, l + 1);
        f[l + 1] = a[i]; 
    }

    cout<<len<<endl;

    return 0;
}