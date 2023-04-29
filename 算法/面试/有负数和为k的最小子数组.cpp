/*
 * @Author: zzzzztw
 * @Date: 2023-04-28 15:01:24
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-28 15:16:40
 * @FilePath: /myLearning/test.cpp
 */
#include <bits/stdc++.h>
using namespace std;    
int subarraySum(vector<int>& nums, int k) {
        unordered_map<int,int>mp;
        int sum = 0;
        int res = INT_MAX;
        mp[0] = 0;
        for(int i = 0; i<nums.size(); i++){
            sum += nums[i];
            if(mp.count(sum - k)){
                res = min(res, i - mp[sum - k]);
                cout<<i<<" ";
            }
            mp[sum] = i;
        }
        return res;
}

int main(){
    vector<int>nums{1,-1,5,-2,3,4,5,6,-3,-1,12};
    int res = subarraySum(nums, 12);
    if(res == INT_MAX)cout<<"-1"<<endl;
    else cout<<res<<endl;
    return 0;
}