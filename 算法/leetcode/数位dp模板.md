<!--
 * @Author: zzzzztw
 * @Date: 2023-03-20 08:39:20
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-20 09:45:02
 * @FilePath: /cpptest/算法/leetcode/数位dp模板.md
-->
# 数位dp
1. leetcode： 233 600 902 1067 1397

leetcode 1012
核心具体模板代码：
```cpp
/*当前枚举从高到低枚举到第i位， 
mask为前面填过数字的集合，不一定有这个参数，选重复/不重复数字会需要这个参数
islimit为前一位数字是否填到了上限，比如123 目前填了12，当前位只能填0 - 3
isnum为前面是否填了数字，如果false这一位可以跳过， 或者要填的数字至少1， 若true 可以填从0开始*/
//后两个参数适用于所有数位dp

//假设当前范围 1e9

//前处理：
string str = to_string(n);
int m = str.size();
const int N = 10;
int f[N][1<<10];
memset(f,-1,sizeof f);
int dfs(int i, int mask, bool islimit, bool isnum) 
{
    if(i == m){
        return isnum;
    }
    if(!islimit && isnum && f[i][mask] != -1)return f[i][mask];

    int res = 0;

    if(!isnum)res = dfs(i+1, mask, false, false) // 如果前一位没填数字，这一位也可以跳过去不填,这一位不填数字下一位自然没有限制


    int up = islimit ? str[i] - '0':9;

    for(int j = isnum ? 0:1;j<=up; j++){
        if((mask >>j & 1) == 0){
            res += dfs(i+1, mask|1<<j, islimit && j == up, true);
        }
    }
    if(!islimit && isnum){
        f[i][mask] = res;
    }
    return res;
}
```

leetcode 233 寻找0 - n中 x出现的次数

```cpp
/*当前枚举从高到低枚举到第i位， 

cnt 为已经出现1的次数
islimit为前一位数字是否填到了上限，比如123 目前填了12，当前位只能填0 - 3
isnum为前面是否填了数字，如果false这一位可以跳过， 或者要填的数字至少1， 若true 可以填从0开始*/
//后两个参数适用于所有数位dp

//假设当前范围 1e9

class Solution {
public:
    static const int N = 1 <<10;
    int f[N][N];// 第i位已经出现x的次数
    int dfs(int i, int cnt, bool islimit, bool isnum, int n, string &str){
        if( i == n)return cnt;
        if(!islimit && isnum && f[i][cnt] != -1)return f[i][cnt];
        int res = 0;
        if(!isnum)res = dfs(i+1, cnt, false , isnum, n , str);

        int up = islimit ? str[i] - '0': 9;

        for(int j = isnum ? 0:1; j<= up ;j++){
            res += dfs(i+1 , cnt + (j == x), islimit && j == up, true, n , str);
        }
        // islimit的情况在整个搜索中只会出现一次，所以不需要记忆化，记忆化的是受到islimit限制的情况下出现x的次数
        if(!islimit && isnum){ 
            f[i][cnt] = res;
        }
        return res;
    }
    int numberOf2sInRange(int n) {
        memset(f,-1,sizeof f);
        string str = to_string(n);
        int m = str.size();
        return dfs(0,0,true, false, m, str);
    }
};

```