<!--
 * @Author: zzzzztw
 * @Date: 2023-04-02 13:53:38
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-02 17:46:30
 * @FilePath: /cpptest/算法/leetcode/区间dp模板.md
-->

# 区间dp




1. lc 1039. 多边形三角剖分的最低得分
```cpp

class Solution {
public:
    static const int N = 55;
    int f[N][N];
    int minScoreTriangulation(vector<int>& values) {
        int n = values.size();
        memset(f,0x3f, sizeof f);
        for(int i = 0;i<n-1;i++){
            f[i][i] = 0;
            f[i][i+1] = 0;
        } 
        for(int i = n-2;i>=0;i--){
            for(int j = i + 1;j<n;j++){
                for(int k = i+1;k<j;k++){
                    f[i][j] = min(f[i][j],f[i][k] + f[k][j] + values[i] * values[k] * values[j]);
                }
            }
        }
        return f[0][n-1];
    }
};

```

## 博弈论dp/区间dp
lc 486 464 877 312