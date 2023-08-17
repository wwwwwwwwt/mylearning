<!--
 * @Author: zzzzztw
 * @Date: 2023-04-08 09:42:35
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-08-17 12:10:37
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


3. 特别的排列

```cpp
class Solution {
public:
    static const int N = 15;
    using ll = long long;
    ll f[1<<N][20];
    static const int MOD = 1e9 + 7;
    int specialPerm(vector<int>& nums) {
        for(int i = 0; i < nums.size(); i++){
            f[1<<i][i] = 1;
        }
        int n = nums.size();
        for(int i = 0; i < 1 << n; i++){
            for(int j = 0; j < n; j++){
                if(i & (1 << j))continue;
                for(int k = 0; k < n; k++){
                    if((i & (1 << k)) && (nums[k] % nums[j] == 0 || nums[j] % nums[k] == 0)){
                        f[i|(1<<j)][j] += f[i][k];
                    }
                }
            }
        }
        ll res = 0;
        for(int i = 0; i < n;i++){
            res += f[(1<<n)-1][i];
        }
        return res % MOD;
    }
};

```
4. 996 正方形数组的数目
```cpp
class Solution {
public:
    static const int N = 15;
    int f[1<<N][15];
    using ll = long long;
    int numSquarefulPerms(vector<int>& nums) {
        for(int i = 0; i < nums.size(); i++){
            f[1<<i][i] = 1;
        }
        int n = nums.size();
        sort(nums.begin(), nums.end());
        for(int i = 0; i < 1 << n; i++){
            for(int j = 0; j < nums.size(); j++){
                if(i & 1 << j)continue;
                int last = -1;
                for(int k = 0; k < nums.size(); k++){
                    if((i & (1 << k))){
                        ll a = sqrt(nums[j] + nums[k]);
                        if(a * a == (nums[j] + nums[k])){
                            if(k > 0 && f[i|(1<<j)][j] > 0 && nums[k] == nums[last])continue;
                            f[i | (1 << j)][j] += f[i][k];
                        }
                        last = k;
                    }
                }
            }
        }
        ll res = 0;
        for(int i = 0; i < nums.size(); i++){
            if(i > 0 && nums[i] == nums[i-1])continue;
            res += f[(1 << n) - 1][i];
        }
        return res;
    }
};

```

5. 2305 公平分发饼干

```cpp

class Solution {
public:
    static const int N = 10;
    int f[10][1<<N];
    int distributeCookies(vector<int>& cookies, int k) {
        int n = cookies.size();
        memset(f, 0x3f, sizeof f);
        int st = ((1 << n) - 1) ^ 0;
        for(int cur = st; cur != 0; cur = (cur - 1) & st){
            int sum = 0;
            for(int num = 0; num < n; num++){
                if(((cur >> num) & 1) == 1){
                    sum += cookies[num];
                }
            }
            f[0][0 | cur] =  sum;
        } 
        for(int i = 0; i < 1 << n; i++){
            for(int j = 1; j < k; j++){
                st = ((1 << n) - 1) ^ i;
                for(int cur = st; cur != 0; cur = (cur - 1) & st){
                    int sum = 0;
                    for(int num = 0; num < n; num++){
                        if((cur >> num & 1) == 1){
                            sum += cookies[num];
                        }
                    }
                    f[j][i | cur] = min(f[j][i | cur], max(f[j-1][i] , sum));
                } 
            }
        }
        return f[k-1][(1<<n) - 1];
    }
};
```

6. 1723 完成所有工作最短时间

```cpp
class Solution {
public:
    static const int N = 15;
    int f[15][1<<N];
    int cnt[1<<N];
    int minimumTimeRequired(vector<int>& jobs, int k) {
        memset(f, 0x3f, sizeof f);
        int n = jobs.size();
        for(int i = 0; i < 1<<n;i++){
            int sum = 0;
            for(int j = 0; j < n; j++){
                if(i >> j & 1){
                    sum += jobs[j];
                }
            }
            cnt[i] = sum;
        }
        for(int i = ((1 << n) - 1); i != 0; i = (i-1)&((1<<n)-1)){
            f[0][0|i] = cnt[i];
        }
        for(int i = 0; i < 1 << n; i ++){
            for(int j = 1; j < k; j++){
                int st = ((1 << n) - 1) ^ i;
                for(int cur = st; cur != 0; cur = (cur - 1) & st){
                    f[j][i | cur] = min(f[j][i|cur], max(f[j-1][i], cnt[cur]));
                }
            }
        }
        return f[k-1][(1<<n) - 1];
    }
};

```

7. 1879 两个数组最小的异或值之和

```cpp
class Solution {
public:
    static const int N = 15;
    int f[15][1<<N];
    int cnt[1<<N];
    int minimumXORSum(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        memset(f, 0x3f, sizeof f);
        for(int i = 0; i < n; i++){
            f[0][1<<i] = nums2[0] ^ nums1[i];
        }
        for(int i = 0; i < 1 << n; i++){
            int sum = 0;
            for(int j = 0; j < n; j++){
                if(i >> j & 1)sum++;
            }
            cnt[i] = sum;
        }      
        for(int i = 0; i < 1 << n ;i++){
            for(int j = 1; j < n; j++){
                if(cnt[i] == j){
                    for(int k = 0; k < n; k++){
                        if((i & (1 << k)) == 0)f[j][i | 1<<k] = min(f[j][i|1<<k], f[j-1][i] + (nums2[j] ^ nums1[k]));
                    }
                }
            }
        }
        return f[n-1][(1<<n)-1];
    }
};
```


8. 2172 数组的最大与和

超时答案：
```cpp
class Solution {
public:
    static const int N = 19;
    int f[10][1<<N];
    int cnt[1<<N];
    int ans[N][1<<N];
    int maximumANDSum(vector<int>& nums, int numSlots) {
        int n =  nums.size();
        for(int i= 0;  i < 1 <<n; i++){
            int sum = 0;
            for(int j = 0; j < n; j++){
                if((i >> j) & 1)sum++;
            }
            cnt[i] = sum;
        }
        for(int i = 0; i < 1<<n; i++){
            if(cnt[i] == 1 || cnt[i] == 2){
                int sum = 0;
                for(int k = 0; k < n; k++){
                    if(((i >> k) & 1) == 1){
                        sum = sum + (nums[k] & 1);
                        
                    }
                }
                f[1][i] = sum;
            }
        }
        for(int j = 2 ; j<= numSlots; j++){
            for(int i = 0; i < 1<<n; i++){
                int sum = 0;
                if(cnt[i] == 1 || cnt[i] == 2){
                    for(int k = 0; k < n; k++){
                        if(((i >> k) & 1) == 1){
                            ans[j][i] = ans[j][i] + (nums[k] & j);
                        }
                    }
                }
            }
        }

        for(int i = 0; i < 1 << n; i++){
            for(int j = 2; j <= numSlots; j++){
                if(cnt[i] > (j - 1) * 2)continue;
                int st = ((1<<n) - 1) ^ i;
                if(cnt[st] > (numSlots - j + 1) * 2)continue;
                f[j][i] = max(f[j][i], f[j-1][i]);
                for(int cur = st; cur != 0; cur = (cur - 1) & st){
                    if(cnt[cur] == 1 || cnt[cur] == 2){
                        f[j][i | cur] = max(f[j][i|cur], f[j-1][i] + ans[j][cur]);
                    }
                }
            }
        }
        int res = 0;
        for(int i = 1; i <=numSlots; i++){
            res = max(res, f[i][(1<<n) - 1]);
        }
        return res;
    }
};
```

```cpp
class Solution {
public:
    static const int N = 19;
    int f[1<<N];
    int maximumANDSum(vector<int>& nums, int numSlots) {
        int n = 2 * numSlots;
        int res=  0;
        for(int i = 0; i < 1 << n; i++){
            int cnt = __builtin_popcount(i);
            if(cnt >= nums.size())continue;
            for(int j = 0; j < n; j++){
                if(i >> j & 1)continue;
                f[i|1<<j] = max(f[i|1<<j], f[i] + (nums[cnt] & (j / 2 + 1)));
                res = max(res, f[i|1<<j]);
            }
        }
        return res;
    }
};


```


9. 1434 每个人不同帽子的方案数量
* 思路：帽子很多，人很少，以人为状态压缩， f[i][j]表示i状态下，遍历到第j个帽子，可以有的方案数

```cpp
class Solution {
public:
    using ll = long long;
    ll f[1 << 11][50];
    vector<int>g[100];
    static const int MOD = 1e9 + 7;
    int numberWays(vector<vector<int>>& hats) {
        set<int>st;
        for(int i = 0; i < hats.size(); i++){
            for(int j = 0; j < hats[i].size(); j++){
                g[hats[i][j]].push_back(i);
            }
        }
        int n = hats.size();
        f[0][0] = 1;
        for(int j = 1; j<= 40; j++){
            for(int i = 0; i < (1 << n); i++){
                f[i][j] += f[i][j-1]; //第j个帽子不选
                for(auto c : g[j]){ //选了
                    if(((i >> c) & 1) == 0){ // 帽子对应的人不在集合中
                        f[i|(1<<c)][j] += f[i][j-1]; // 状态转移过去，第j个帽子选了 更新新的状态。
                    }
                }
            }
        }
        return f[(1<<n) - 1][40] %MOD;
    }
};


```