<!--
 * @Author: zzzzztw
 * @Date: 2023-04-08 09:42:35
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-08 09:45:33
 * @FilePath: /cpptest/算法/leetcode/状态压缩dp.md
-->
# 状态压缩dp

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