/*
 * @Author: zzzzztw
 * @Date: 2023-07-05 08:35:32
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-07-05 14:18:23
 * @FilePath: /myLearning/算法/面试/数位dp统计不出现101的个数.cpp
 */

/*小明在学习二进制时，发现了类不含101的数，也就是将数字用二进制表示，不能出现101.现在给定一个正整数区间[门，请问这个区间内包合了多少个不含101的数?例子:
输入:
l = 1, r = 10
输出:
8
解释:
满足条件的有: [1,2,3,4,6,7,8,9]
输入:
l= 1,r = 20
输出:
解释:
满足条件的有: [12,14,15,16,17,18,19*/
#include <bits/stdc++.h>
using namespace std;
const int N = 32;
int f[N][N];
int help(string num){
    int res = 0;
    for(int i = 0; i < (int)num.size(); i++){
        res  = res * 10 + (num[i] - '0');
    }
    return res;
}
int dfs(int i, string cnt, int pre, bool ispattern, bool islimit,const string& str){
    if(i == (int)str.size())return 1;
    int num = -1;
    num = help(cnt);
    if(!islimit && num != -1 &&f[i][num] != -1)return f[i][num];
    int res = 0;
    int up = islimit ? (str[i] - '0') : 1;
    int up2 = ispattern ? 0 : 1;
    if(i < 1){
        for(int j = 0; j <= up; j++){
            res += dfs(i + 1, to_string(j), j, false, islimit && (j == up),  str);
        }   
    }
    else{
        for(int j = 0; j <=min(up, up2); j++){
            res += dfs(i + 1, to_string(pre) + to_string(j), j, pre == 1 && j == 0, islimit && (j == up),  str);
        }
    }

    if(num != -1 && !islimit) f[i][num] = res;
    return res;
}
int main(){
    int l, r;
    cin>>l>>r;
    l = l - 1;

    string str = "";
    while(r){
        str += to_string(r & 1);
        r >>=1;
    }
    
    reverse(str.begin(), str.end());
    memset(f, -1, sizeof f);
    int res2 = dfs(0,"", 0,false, true, str);

    str = "";
    while(l){
        str +=  to_string(l & 1);
        l >>=1;
    }
    reverse(str.begin(), str.end());
    memset(f, -1, sizeof f);
    int res1 = dfs(0,"", 0,false, true,str);

    cout<<res2 - res1<<endl;
    return 0;
}