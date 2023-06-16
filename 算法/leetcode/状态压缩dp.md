<!--
 * @Author: zzzzztw
 * @Date: 2023-04-08 09:42:35
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-16 10:30:46
 * @FilePath: /myLearning/算法/leetcode/状态压缩dp.md
-->
# 状态压缩dp


* 技巧
* https://leetcode.cn/circle/discuss/CaOJ45/
```cpp
1. 统计这个状态中有多少个1， 一般用于预处理所有状态
for(int i = 1; i < totalst; i++){
    cnt[i] = cnt[i>>1] + (i&1);
}

2. 枚举状态的所有子集
for(int sub = cur; sub != 0; sub = (sub - 1)&cur){
    
}

3. 查看这个状态x是不是另一个状态y的子集
if((x & y) == x){
    
}

4. 判断状态中是否有连续的1, true为没有连续1
if((x & (x>>1)) == 0){

}

5. 判断第i位是否为1, true为第i位是1
if((x >> i) & 1){

}
```

1. lc 1125 最小的必要团队
* 思路：f[i]记录着i这个覆盖技能的最小方案，g[i]代表这个人的技能覆盖， path记录状态i|g[i]是从i状态，用了j号人转移过来的

```cpp
class Solution {
public:
    using pii = pair<int,int>;
    vector<int> smallestSufficientTeam(vector<string>& req_skills, vector<vector<string>>& people) {
        int n =  req_skills.size(), m = people.size();
        vector<int>f(1<<n,n+1), g(m);
        vector<pii>path(1<<n+1);
        unordered_map<string, int>mp;
        for(int i = 0;i<req_skills.size();i++)mp[req_skills[i]] = i;

        for(int i = 0;i<people.size();i++){
            for(auto & c: people[i]){
                g[i] |= 1<<mp[c];
            }
        }
        f[0] = 0;
        for(int i = 0;i<1<<n;i++){
            for(int j = 0;j<people.size();j++){
                int &v = f[i|g[j]];
                if(v > f[i] + 1){
                    v =  f[i] + 1;
                    path[i|g[j]] = {i,j};
                }
            }
        }
        vector<int>res;
        for(int i =  (1<<n) - 1;i;){
            res.push_back({path[i].second});
            i = path[i].first;
        }

        return res;
    }
};

```

2. 1494 并行课程2

```cpp
class Solution {
public:
    static const int N = 16;
    int f[1<<N];
    int cnt[1<<N];
    int precourse[N];
    int minNumberOfSemesters(int n, vector<vector<int>>& relations, int k) {
        for(int i = 0; i < relations.size(); i++){
            auto t = relations[i][0]-1, e = relations[i][1]-1;
            precourse[e] |= 1 << (t);
        }

        int st = 1 << n;
        cnt[0] = 0;
        for(int i = 1; i < st; i++){
            cnt[i] = cnt[i >> 1] + (i & 1);
        }
        memset(f, 0x3f, sizeof f);
        f[0] = 0;
        //枚举状态，状态为当前上了哪些课
        for(int i = 0; i < st; ++i){
            if(f[i] > n)continue;
            int cur = 0;
            for(int j = 0; j < n; ++j){
                if( ((i & (1 << j) ) == 0) &&(precourse[j] & i) == precourse[j]  ){
                    //当前学期可以上这门课
                    cur |= (1 << j);
                }
            }

            for(int sub = cur; sub !=  0; sub = sub - 1 & cur){
                if(cnt[sub] <= k){
                    f[i|sub] = min(f[i|sub], f[i] + 1);
                }
            }
        }

        return f[st - 1];
    }
}

```