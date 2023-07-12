<!--
 * @Author: zzzzztw
 * @Date: 2023-03-29 10:16:28
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-07-11 11:03:37
 * @FilePath: /myLearning/算法/leetcode/线性dp.md
-->
# 线性dp 在这里分为 递推类型，二维矩阵类型， 打家劫舍，状态机类型（买卖股票）

## 递推类型

1. 丑数2

```cpp
class Solution {
public:
    static const int N = 1700;
    int f[N];
    int nthUglyNumber(int n) {
        f[0] = 1;
        int q2 = 0, q3 = 0, q5 = 0;
        for(int i = 1;i<n;i++){
            f[i] = min(f[q2] * 2, min(f[q3] * 3, f[q5] * 5));
            if(f[i] == f[q2] * 2)q2++;
            if(f[i] == f[q3] * 3)q3++;
            if(f[i] == f[q5] * 5)q5++;
        }
        return f[n-1];
    }
};

```

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

3. 分隔数组以得到最大和 lc1043

* 思路：记忆化搜索dp，枚举当前k内的最大值 + 前面范围最大和 就是当前最大值
* 注意这类题一般f下标都比数组下标大一个

```cpp
class Solution {
public:
    static const int N = 510;
    int f[N];
    void dfs(int idx, int k, vector<int>&arr){
        if(idx == arr.size()+1)return;
        int maxv = -1;
        for(int i = idx; idx - i < k && i >0;i-- ){
            maxv = max(maxv, arr[i-1]);
            f[idx] = max(f[idx], f[i-1] + (idx - i +1) * maxv);
        }
        dfs(idx +1 ,k , arr);
    }
    int maxSumAfterPartitioning(vector<int>& arr, int k) {
        memset(f, -1, sizeof f);
        f[0] = 0;
        dfs(1,k,arr);
        return f[arr.size()];
    }
};


```

4. 检查数组是否存在有效划分 lc2369

* 思路同上，dp 枚举当前范围是否符合要求 + 前面范围符合要求，就是当前范围，注意这类题下标比数组大一个

```cpp
class Solution {
public:
    static const int N = 100010;
    bool f[N];
    bool validPartition(vector<int>& nums) {
        f[0] = true;
        for(int i = 2;i<=nums.size();i++){
            f[i] |= f[i-2] && nums[i-2] == nums[i-1];
            if(i>=3){
                f[i] |= f[i-3] &&(nums[i-3] == nums[i - 2]-1 && nums[i - 2] == nums[i-1] - 1);
                f[i] |= f[i-3] &&(nums[i-3] == nums[i - 2]  && nums[i - 2]== nums[i-1] );
            }
        }
        return f[nums.size()];
    }
};

```


5. 剑指offer60 n个筛子的点数

* 思路：f[i][j]是第i个筛子和为j的方案数，为前一个筛子减去1 - 6当前筛子的值的方案数总和，当j < k就break，总次数有pow（6，n）种
  
```cpp

class Solution {
public:
    static const int N = 15;
    int f[N][6*N];
    vector<double> dicesProbability(int n) {
        for(int i = 1;i<=6;i++)f[1][i] = 1;
        for(int i = 2; i<=n;i++){
            for(int j = i; j<=6*i;j++){
                for(int k = 1; k<=6;k++){
                    if( j < k)break;
                    f[i][j] += f[i-1][j-k];
                }
            }
        }
        double total = pow(6, n);
        vector<double>res;
        for(int i = n; i<=6*n;i++){
            res.push_back(f[n][i] / total);
        }
        return res;
    }
};


```

6. 最大子序交替和1911

```cpp
class Solution {
public:
    using ll = long long;
    static const int N = 100010;
    ll f[N][2][2];
    long long maxAlternatingSum(vector<int>& nums) {
        f[0][1][1] = nums[0];
        f[0][0][0] = 0;
        f[0][1][0] = f[0][0][1] = INT_MIN;
        for(int i = 1; i < nums.size(); i++){
            f[i][1][1] = max(f[i-1][1][0], f[i-1][0][0]) + nums[i];
            f[i][0][1] = max(f[i-1][0][1], f[i-1][1][1]);
            f[i][1][0] = max(f[i-1][1][1], f[i-1][0][1]) - nums[i];
            f[i][0][0] = max(f[i-1][0][0], f[i-1][1][0]);
        }
        int n = nums.size();
        return max(f[n-1][1][1], f[n-1][0][1]);
    }
};
```