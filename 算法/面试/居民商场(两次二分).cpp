/*
 * @Author: zzzzztw
 * @Date: 2023-03-28 21:15:16
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-10 09:38:07
 * @FilePath: /myLearning/算法/面试/居民商场(两次二分).cpp
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 50010;
int a[N],b[N];
int pos[N+N];
int cnt[N];
int main(){
    int n,m;
    cin>>n>>m;

    for(int i = 0;i<n+m;i++){
        cin>>pos[i];
    }
    int j = 0, k = 0;
    for(int i= 0;i<n+m;i++){
        int t ;
        cin>>t;
        if(t)a[j++] = pos[i]; //a商场
        else b[k++] = pos[i]; //
    }

    sort(a,a+m);
    for(int i = 0;i<n;i++){
        int target = b[i];
        int l = 0, r = m-1;
        while(l < r){
            int mid = l + r + 1 >> 1;
            if(a[mid] <= target) l = mid;
            else r = mid - 1;
        }
        int pos1 = l >=0 ? a[l] : INT_MAX;

        int left = 0, right = m - 1;
        while (left < right)
	    {
            int mid = left + right >> 1;
            if(a[mid] < target) left = mid + 1;
            else right = mid;
	    }
        int pos2 = left <= m - 1? a[left]: INT_MAX;

        if(abs(pos1 - target) == abs(pos2 - target)){
            cnt[r]++;
        }
        else if(abs(pos1 - target) < abs(pos2 - target)){
            cnt[r]++;
        }
        else{
            cnt[left]++;
        }
    }

    for(int i = 0;i<m;i++){
        cout<<cnt[i]<<" ";
    }
    return 0;
}

/*
二分 

1. 小于等于目标得最后一个

while(l < r){
    int mid = l + r +1 >>1;
    if(a[mid] <= target) l = mid;
    else r = mid - 1;
}

int res = l >=0 ?a[l] : -1;

2. 小于目标得最后一个

while(l<r){
    int mid = l + r + 1>>1;
    if(a[mid] < target)l = mid;
    else r = mid - 1;
}
int res = l >=0 ?a[l] : -1;

3. 大于等于目标第一个

while(l < r){
    int mid = l + r >> 1;
    if(a[mid] < target)l = mid + 1;
    else r = mid;
}


4. 大于目标第一个

while(l< r){
    int mid = l + r >> 1;
    if(a[mid] <= target) l = mid + 1;
    else r = mid;
}

*/