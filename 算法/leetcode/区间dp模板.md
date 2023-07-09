<!--
 * @Author: zzzzztw
 * @Date: 2023-04-02 13:53:38
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-07-09 09:28:19
 * @FilePath: /myLearning/算法/leetcode/区间dp模板.md
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

2. 1547 切棍子的最小成本


```cpp
class Solution {
public:
    static const int N = 110;
    int f[N][N];
    int minCost(int n, vector<int>& cuts) {
        cuts.insert(cuts.begin(), 0);
        cuts.push_back(n);
        n = cuts.size();
        sort(cuts.begin(), cuts.end());
        for(int len = 2; len <= n; len++){
            for(int l = 0; l + len  < n; l++){
                int r = l + len;
                f[l][r] = INT_MAX;
                for(int k = l+1; k < r; k++){
                    f[l][r] = min(f[l][r], f[l][k] + f[k][r] + cuts[r] - cuts[l]);
                }
            }
        }
        return f[0][n-1];
    }
};

3. 合并石子 acwing

```cpp
const int N = 310;
int f[N][N];
int a[N],s[N];
int main(){
    int n;
    cin>>n;
    for(int i = 1;i<=n;i++)cin>>a[i];
    for(int i = 1;i<=n;i++)s[i] = s[i-1] + a[i];
    memset(f,0x3f,sizeof f);
    for(int len = 1;len<=n;len++){
         for(int l = 1;l + len -1<=n;l++){
            int r = l +len -1;
            if(len == 1)f[l][r] = 0;
            else{
                for(int k = l;k<r;k++){
                    f[l][r] = min(f[l][r],f[l][k]+f[k+1][r] + s[r] - s[l-1]);
                }                      
            }
         }
    }
    cout<<f[1][n];
    return 0;
    
}

```



```


## 博弈论dp/区间dp
lc 486 464 877 312