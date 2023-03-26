<!--
 * @Author: zzzzztw
 * @Date: 2023-03-22 09:14:16
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-22 11:18:35
 * @FilePath: /cpptest/算法/leetcode/最长上升子序列dp&&二分.md
-->
# 最长上升子序列
最长上升子序列为二维dp和二分贪心两种做法，
* 求最长长度两种都可以做
* 求最长上升子序列的费用一般只能dp（如子序列加起来的数值和等）
* lc典型题目：lc300，1691堆箱子，2008出租车最大盈利（dp+二分优化，与二分贪心无关）1235规划兼职工作（dp+二分优化，与二分贪心无关）
* 二分贪心:354俄罗斯套娃，面试题17.08马戏团人塔
* acwing 典型题目：1017怪盗基德滑翔翼 1014登山 482合唱队形，1012友好城市
## 最普通二维dp做法
```cpp
给你一个整数数组 nums ，找到其中最长严格递增子序列的长度。
子序列 是由数组派生而来的序列，删除（或不删除）数组中的元素而不改变其余元素的顺序。  
例如，[3,6,2,7] 是数组 [0,3,1,6,2,2,7] 的子序列。

#include <bits/stdc++.h>
using namespace std;
const int N = 2510;
int a[N],f[N];
int main(){
    int n;
    cin>>n;
    for(int i = 0;i<n;i++)cin>>a[i];

    int res = 0;
    for(int i = 0;i<n;i++){
        f[i] = 1;
        for(int j = 0;j<i;j++){
            if(a[j] < a[i]){
                f[i] = max(f[i], f[j] + 1);
            }
            res = max(res, f[i]);
        }
    }
    cout<<res<<endl;
    return 0;
}

```
## 最普通二分贪心做法
* f[i]表示长度为i的子序列，末尾元素大小
* 注意我们定义的是找到最后一个小于a[i]的点，所以二分时与普通模板不同  
* 让r = mid - 1, r在一定， mid < a[i] 时 l = mid
* 每次二分完，len = max(len,l + 1),接着更新这个序列长度和末尾元素f[l + 1] = a[i];

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 2510;
int a[N],f[N];

int main(){
    int n;
    cin>>n;
    for(int i = 0;i<n;i++)cin>>a[i];

    int len = 0;

    for(int i = 0;i<n;i++){
        int l = 0, r = len;
        while(l<r){
            int mid = l + r + 1 >>1;
            if(f[mid] < a[i])l = mid;
            else r = mid - 1;
        }
        len = max(len, l + 1);
        f[l + 1] = a[i]; 
    }

    cout<<len<<endl;

    return 0;
}

```

## 动规典型题目
1. lc1691 堆箱字
* 条件给了旋转，就表示我们可以对长宽高进行排序，预处理相当于把箱子摆正
* 按照长宽高和的大小进行排序，使其有序否则三个变量无序无法进行dp
* 套一个二维dp模板就过了
```

```

2. 2008 出租车最大盈利
* 由于数据量，O(n^2)会超时，所以需要进行一定优化
* 老套路将三元组合并到一个数组[start, end, 可以获得的费用：end - statr + fee]
* 老套路， 排序，我们按照终止点end，从小到大排序
* 由于我们将end从小到大排序了，所以我们可以快速找到可以使用二分快速找到一个点的end小于当前start
* 此时分两种情况：
* 1.找到了前面有一个区间与现在起点不重叠：接受当前订单 f[l] + a[i][2], 不接受当前订单，最大值为之前做的订单最大值f[i-1];max(f[i-1], f[l] + a[i][2]);
* 2.没找到前面与现在起点不重叠区间，接受当前订单a[i][2], 最大值为之前做的订单最大值f[i-1], f[i] = max(f[i-1], a[i][2]);
* 特别注意：这个二分和贪心二分不一样，我们需要找到最后一个终点end小于当前起始点的位置 <=，所以移动r
```cpp
  mid = l +r >>1; 
  if(mid <= target)l = mid;
  else r = mid - 1;
```
* 代码
```cpp

static const int N = 100010;
long long f[N];
long long maxTaxiEarnings(int n, vector<vector<int>>& rides) {
    vector<vector<long long>>a;
    for(int i = 0;i<rides.size(); i++){
        a.push_back({rides[i][0],rides[i][1], rides[i][1] - rides[i][0] + rides[i][2]});
    }

    sort(a.begin(),a.end(),[](auto &a1, auto &b1){
        return a1[1] < b1[1];
    });

    f[0] = a[0][2];

    for(int i = 1;i<a.size();i++){
        int l = 0, r = i - 1;
        while(l<r){
            int mid = l + r + 1>>1;
            if(a[mid][1] <= a[i][0]) l = mid;
            else r = mid - 1;
        }
        if(a[l][1] <= a[i][0]){
            f[i] = max(f[i-1], f[l] + a[i][2]);
        }
        else f[i] = max(f[i-1], a[i][2]);
    }

    return f[rides.size()-1];
};


```
3. 规划兼职工作，思路同上，将结束时间从小到大排序
```cpp

    static const int N = 50050;
    int f[N];
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        vector<vector<int>>g;
        int n = startTime.size();
        for(int i = 0;i<n;i++){
            g.push_back({startTime[i],endTime[i],profit[i]});
        }
        sort(g.begin(),g.end(),[&](auto &a,auto &b){
            return a[1]<b[1];
        });

        f[0] = g[0][2];
        for(int i = 1;i<n;i++){
            int l = 0,r = i-1;
            while(l<r){
                int mid = l + r +1>>1;
                if(g[mid][1]<=g[i][0])l = mid;
                else r = mid -1;
            }
            if(g[l][1]<=g[i][0])f[i] = max(f[i-1],f[l] + g[i][2]);
            else f[i] = max(f[i-1],g[i][2]);
        }
        return f[n-1];

    }

```

## 二分贪心典型题目
1. 354 俄罗斯套娃
* 最重要的点！按照宽从小到大排序，如果宽相等，按照长从大到小排序，防止二分时会出现 宽相等但长度后一个比前一个计算多次的错误答案

```cpp

```
2. 马戏团思路一模一样同上

## acwing题目
1. 怪盗基德
2. 登山
3. 友好城市
*  排序一边，另一边上升子序列二维dp