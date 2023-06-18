<!--
 * @Author: zzzzztw
 * @Date: 2023-06-15 18:35:09
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-17 08:53:09
 * @FilePath: /myLearning/算法/leetcode/换根&树形dp.md
-->
# 树型dp

1. 打家劫舍3 337

```cpp
class Solution {
public:
    vector<int>dfs(TreeNode* root){
        if(root == nullptr){
            return {0,0};
        }
        vector<int>left = dfs(root->left);
        vector<int>right = dfs(root->right);

        int val0 = root->val + left[1] + right[1];
        int val1 = max(left[0], left[1]) + max(right[0], right[1]);

        return {val0, val1};
 
    }
    int rob(TreeNode* root) {
        vector<int>res = dfs(root);
        return max(res[0], res[1]);
    }
};
```

2. 树的直径 1245

```cpp
class Solution {
public:
    int res;
    static const int N = 10010;
    vector<int>g[N];
    int dfs(int u,int father){
        int d1 = 0, d2 = 0;
        for(auto &c:g[u]){
            if(c == father)continue;
            int d = dfs(c,u)+1;
            if(d>=d1){
                d2 = d1;
                d1 = d;
            }
            else if(d>d2){
                d2 = d;
            }
        }
        res = max(res,d1+d2);
        return d1;
    }
    int treeDiameter(vector<vector<int>>& edges) {
        for(int i = 0;i<edges.size();i++){
            int x = edges[i][0], y = edges[i][1];
            g[x].push_back(y);
            g[y].push_back(x);
        }
        dfs(0,-1);
        return res;
    }
};

```

# 换根dp 

1. 树中距离之和 834
* 第一遍从上到下正常深搜，先深度，再来更新父节点。第二遍从上到下，先用更新父节点，更新好了再深搜，子节点需要更新过的父节点值。
```cpp
class Solution {
public:
    static const int N = 30010;
    vector<int>g[N];
    int sum[N], cnt[N], up[N];
    void dfs1(int u, int father){
        sum[u] = 0;
        cnt[u] = 1;
        for(auto c : g[u]){
            if(c == father)continue;
            dfs1(c, u);
            sum[u] += sum[c] + cnt[c];
            cnt[u] += cnt[c];
        }
    }
    void dfs2(int u, int father, int n){
        
        for(auto c : g[u]){
            if(c == father)continue;
            up[c] = up[u] + sum[u] - (sum[c] + cnt[c]) + n - cnt[c];
            dfs2(c, u, n); 
        }
    }
    vector<int> sumOfDistancesInTree(int n, vector<vector<int>>& edges) {
        for(int i = 0; i < edges.size(); i++){
            auto a = edges[i][0], b = edges[i][1];
            g[a].push_back(b);
            g[b].push_back(a);
        }
        dfs1(0,-1);
        dfs2(0, -1, n);
        vector<int>res;
        for(int i = 0; i < n;i++){
            res.push_back(up[i] + sum[i]);
        }
        return res;
    }
};
```

