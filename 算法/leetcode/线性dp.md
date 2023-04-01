<!--
 * @Author: zzzzztw
 * @Date: 2023-03-29 10:16:28
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-29 10:21:55
 * @FilePath: /cpptest/算法/leetcode/线性dp.md
-->
# 线性dp 在这里分为 递推类型，二维矩阵类型， 打家劫舍，状态机类型（买卖股票）

## 递推类型

1. 丑数2

2.  统计字典序元音字符串的数目 lc 1641
* 思路：递推：每个字母加在（他这个字母，和比他字典序小一个的字母）后面。
* 得到方案数就是上一轮这个字母和小于这个字母的和
```cpp
/*
给你一个整数 n，请返回长度为 n 、仅由元音 (a, e, i, o, u) 组成且按 字典序排列 的字符串数量。
字符串 s 按 字典序排列 需要满足：对于所有有效的 i，s[i] 在字母表中的位置总是与 s[i+1] 相同或在 s[i+1] 之前。

输入：n = 2
输出：15
解释：仅由元音组成的 15 个字典序字符串为
["aa","ae","ai","ao","au","ee","ei","eo","eu","ii","io","iu","oo","ou","uu"]
注意，"ea" 不是符合题意的字符串，因为 'e' 在字母表中的位置比 'a' 靠后

*/

class Solution {
public:
    static const int N = 5;
    uint64_t f[N];// 4 a 3 e 2 i 1 e 0 u
    int countVowelStrings(int n) {

        for(int  i = 0;i < 5;i++)f[i] = 1;

        for(int i = 2;i<=n;i++){
            for(int j = 1;j<5;j++){
                f[j] = (f[j-1] + f[j]);
            }
        }
        uint64_t res = 0;
        for(int i = 0;i<5;i++)res+=f[i];
        return res;

    }
};

```