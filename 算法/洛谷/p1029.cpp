/*
 * @Author: zzzzztw
 * @Date: 2023-09-10 17:59:49
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-10 18:04:55
 * @FilePath: /myLearning/算法/洛谷/p1029.cpp
 */
#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int b){
    return b ? gcd(b, a % b) : a;
}

int lcm(int a, int b){
    return a / gcd(a, b) * b;
}

int main(){
    int a, b;
    cin>>a>>b;
    int res = 0;
    for(int  i = a; i <= b; i++){
        int j = a * b / i;
        if(gcd(i ,j) == a && lcm(i, j) == b)res++; 
    }
    cout<<res<<endl;
    return 0;
}