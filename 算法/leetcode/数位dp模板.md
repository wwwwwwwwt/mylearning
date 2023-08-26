<!--
 * @Author: zzzzztw
 * @Date: 2023-03-20 08:39:20
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-08-20 09:44:32
 * @FilePath: /myLearning/算法/leetcode/数位dp模板.md
-->
# 数位dp
1. leetcode： 233 600(finish) 902 1067 1397

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

leetcode 1012 至少有 1 位重复的数字

```cpp
class Solution {
public:
    static const int N = 10;
    int f[N][1<<10];
    int dfs(int i,int mask, bool islimit, bool isnum,int n, string s){
        if(i == n){
            return isnum;
        }
        if(!islimit && isnum && f[i][mask] != -1)return f[i][mask];
        uint64_t res = 0;
        if(!isnum){
            res += dfs(i+1, mask, false, isnum , n, s);
        }
        int up = islimit ? s[i] - '0': 9;

        for(int j = isnum ? 0: 1; j<=up;j++){
            if((mask >> j &1) == 0)
            res += dfs(i+1, mask|(1<<j), islimit && j == up ,true, n, s);
        }
        if(!islimit&& isnum){
            f[i][mask] = res;
        }
        return res;

    }
    int numDupDigitsAtMostN(int n) {
        memset(f,-1,sizeof f);
        string str = to_string(n);
        int m = str.size();
        return n - dfs(0,0,true,false,m,str);
    }
};
```

leetcode 600

```cpp
class Solution {
public:
    static const int N = 32;
    int f[N][2];
    int dfs(int i, bool islimit,bool fill, string &str){
        if(i == str.size())return 1;
        if(!islimit && f[i][fill] != -1)return f[i][fill];
        int res = 0;
        int up = islimit ? (str[i] - '0') : 1;
        int up2 = fill ? 0 : 1;
        for(int j = 0; j <= min(up, up2); j++){
            res += dfs(i + 1,  islimit && (j == up),  j == 1, str);
        }
        if(!islimit)f[i][fill] = res;
        return res;
    }
    int findIntegers(int n) {
        string str;
        while(n){
            str += to_string(n & 1);
            n>>=1;
        }
        reverse(str.begin(), str.end());
        memset(f, -1, sizeof f);
        return dfs(0,  true,  false, str);
    }
};


```

111 双周赛t4

* 问数能不能整除k，且奇数偶数位数相同，记忆化i，奇数偶数的cnt， 模k的余数val

```cpp
class Solution {
public:
    static const int N = 10;   
    int f[N][N][N][30];
    int dfs(int i, int cnt1, int cnt2, int val, long long curnum, bool islimit, bool isnum, string &str, int k){
        if(i == str.size())return isnum &&  (curnum % k == 0) && (cnt1 == cnt2);
        if(!islimit && isnum && f[i][cnt1][cnt2][val] != -1)return f[i][cnt1][cnt2][val];
        int res = 0;
        if(!isnum)res += dfs(i+1, 0,0,0,0,false, false, str, k);
        int up = islimit ? str[i] - '0' : 9;
        for(int j = isnum ? 0 : 1; j<=up; j++){
            res += dfs(i + 1, cnt1 + (j%2 == 1), cnt2 + (j%2 == 0), (curnum * 10 + j)% k, curnum * 10 + j, islimit && (j == up),true, str, k);
        }
        if(!islimit && isnum)f[i][cnt1][cnt2][val] = res;
        return res;
    }
    int numberOfBeautifulIntegers(int low, int high, int k) {
        memset(f, -1, sizeof f);
        string str1 = to_string(high);
        int res1 = dfs(0,0,0,0,0,true, false, str1, k);
        memset(f, -1, sizeof f);
        string str2 = to_string(low - 1);
        int res2 = dfs(0,0,0,0,0,true, false, str2, k);
        return res1-res2;
    }
};

```