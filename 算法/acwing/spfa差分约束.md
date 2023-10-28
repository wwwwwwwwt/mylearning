<!--
 * @Author: zzzzztw
 * @Date: 2023-10-25 20:50:57
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-10-26 12:21:45
 * @FilePath: /myLearning/算法/acwing/spfa差分约束.md
-->
# 差分约束，解决一连串不等式传递的问题，并给出每个变量的最大值或最小值。
* 结论1：若出现负环则无解
* 结论2：最小值，求最长路
* 结论3：最大值，求最短路 
```
差分约束建边方法：
如果求最小值，就要求图中的最长路  
将不等式转化为x1+c<=x2的形式，建边为从x1到x2建立边权为c的边  
如果求最大值，就要求图中的最短路  
将不等式转化为x1+c>=x2的形式，建边为从x1到x2建立边权为c的边  
```  


```cpp
    memset(dis, -0x3f, sizeof dis);
    for(auto &c : g[t]){
        if(dis[c.first] < dis[t] + c.second){
            dis[c.first] = dis[t] + c.second;
            cnt[c.first] = cnt[t] + 1;
            if(cnt[c.first] >= n + 1)return false;//求负环，加上虚拟远点需要大于等于n + 1条边。
            if(!s[c.first]){
                st.push_back(c.first);
                s[c.first] = true;
            }
        }
    }
```

```cpp
    memset(dis, 0x3f, sizeof dis);
    for(auto &c : g[t]){
        if(dis[c.first] > dis[t] + c.second){
            dis[c.first] = dis[t] + c.second;
            cnt[c.first] = cnt[t] + 1;
            if(cnt[c.first] >= n + 1)return false; // 求负环，加上虚拟远点需要大于等于n + 1条边。
            if(!s[c.first]){
                st.push_back(c.first);
                s[c.first] = true;
            }
        }
    }

```