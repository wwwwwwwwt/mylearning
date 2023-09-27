/*
 * @Author: zzzzztw
 * @Date: 2023-09-26 19:06:59
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-26 19:11:29
 * @FilePath: /myLearning/算法/洛谷/洛谷题单1-25（finish）/p1628.cpp
 */
#include <bits/stdc++.h>
using namespace std;
vector<string>str;
int main(){
    int n;
    cin>>n;
    for(int i = 0; i < n; i++){
        string s;
        cin>>s;
        str.push_back(s);
    }
    string k;
    cin>>k;
    vector<string>res;
    for(int i = 0; i < str.size(); i++){
        bool flag = true;
        for(int j = 0; j < k.size(); j++){
            if(str[i][j] != k[j]){
                flag = false;
                break;
            }
        }
        if(flag)res.push_back(str[i]);
    }
    sort(res.begin(), res.end());
    for(int i = 0; i < res.size(); i++)cout<<res[i]<<endl;
    return 0;
}