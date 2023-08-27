<!--
 * @Author: zzzzztw
 * @Date: 2023-08-27 20:57:03
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-08-27 21:05:50
 * @FilePath: /myLearning/算法/leetcode/lca相关,.md
-->
# lca 树上倍增

1. 最简单模板题 1483
* 思路：动态规划倍增思想
```cpp
class TreeAncestor {
public:
    int pa[50050][35];
    TreeAncestor(int n, vector<int>& parent) {
        memset(pa, -1, sizeof pa);
        for(int i = 0; i < parent.size(); i++){
            pa[i][0] = parent[i];
        }
        for(int j = 1; j <= 32; j++){
            for(int i = 0; i < parent.size(); i++){
                if(pa[i][j-1] == -1)continue; 
                pa[i][j] = pa[pa[i][j-1]][j-1];
            }
        }
    }
    
    int getKthAncestor(int node, int k) {
        for(int j = 0; j < 32; j++){
            if((k >> j) & 1){
                node = pa[node][j];
                if(node < 0)break;
            }
        }
        return node;
    }
};


```

1. 在传球游戏中最大化函数值 2836


* 思路：pa记录i节点跳j次后的父节点，f记录i节点跳j次后的路径值，但不包括跳j次后终点值，所以最后得加上
```cpp
class Solution {
public:
    long long getMaxFunctionValue(vector<int>& receiver, long long k) {
        int pa[100010][35];
        long long f[100010][35];
        for(int i = 0; i < receiver.size(); i++){
            pa[i][0] = receiver[i];
            f[i][0] = i;
        }
        for(int j = 1; j < 35; j++){
            for(int i = 0; i < receiver.size(); i++){
                pa[i][j] = pa[pa[i][j-1]][j-1];
                f[i][j] = f[i][j - 1] + f[pa[i][j-1]][j-1];
            }
        }
        long long ans = 0;
        for(int i = 0; i < receiver.size(); i++){
            int pos = i;
            long long cur = 0;
            for(int j = 0; j < 35; j++){
                if((k >> j) & 1){
                    cur += f[pos][j];
                    pos = pa[pos][j];
                }
            }
            ans = max(ans, cur + pos);
        }
        return ans;
    }
};

```