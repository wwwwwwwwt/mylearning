/*
 * @Author: zzzzztw
 * @Date: 2023-04-27 20:30:37
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-27 20:39:42
 * @FilePath: /myLearning/算法/面试/给一个字符串，增添最小的字母成为回文串.cpp
 */

/*

给一个字符串，需要去掉一些字母，使这个字符串是一个回文字符串，求去掉最少的字母

思路代码和1312一模一样，1312为插入字符，使其成为回文串

*/

#include <bits/stdc++.h>

using namespace std;

const int N = 510;
int f[N][N];

int func(const string &s){

    for(int i = s.size() - 2; i>=0; i--){
        for(int j = i+1; j<s.size();j++){
            f[i][j] = min(f[i+1][j], f[i][j-1]) + 1;
            if(s[i] == s[j]){
                f[i][j] = min(f[i][j], f[i+1][j-1]);
            }
        }
    }
    return f[0][s.size() - 1];

}


int main(){
    string s;
    cin>>s;
    cout<<func(s)<<endl;
    return 0;
}