<!--
 * @Author: zzzzztw
 * @Date: 2023-03-22 10:58:28
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-27 11:30:16
 * @FilePath: /cpptest/算法/leetcode/字符串类型dp.md
-->
1. 5 最长回文子串
* 思路：双重循环，外层循环i循环当前字符串s[i], 内层循环从i开始，j大于等于0，内层倒着遍历，因为后面f[i][j]需要从f[i-1][j+1]转移回来，j+1应该先被更新.
* 如果i等于j 说明只有一个字符，必然回文 f[i][j] = 1;  
* 如果i -1 == j，并且s[i] == s[j] 说明挨着俩字符相同，肯定回文 f[i][j] =  2；
* 如果 i-1 >= j+1，并且s[i] == s[j] 说明i,j中间有其他字符串，如果这个f[i-1][j+1]存在(一开始全设为0)，那么f[i][j] = f[i-1][j+1] + 2;
* 维护一个res，当res被更新时，记录当前j，i坐标，即为最长回文串的起始点和终止点
```cpp
 static const int N = 1010;
 int f[N][N];
 string longestPalindrome(string s) {

     int res  = 0, start = 0 , end = 0;
     int n = s.size();
     memset(f,0,sizeof f);
     for(int i = 0;i<n;i++){
         for(int j = i;j>=0;j--){
             if(i == j)f[i][j] = 1;
             else if(i == j+1&&s[i] == s[j])f[i][j] = 2;
             else if(i-1 >= j+1&&s[i] == s[j] && f[i-1][j+1]) f[i][j] = f[i-1][j+1] + 2;
             res = max(res,f[i][j]);
             if(res == f[i][j]){
                 start = j;
                 end = i;
             }
         }
     }
     string ans;
     for(int i = start; i<=end;i++)ans += s[i];
     return ans;
 }

```