/*
 * @Author: zzzzztw
 * @Date: 2023-03-28 21:13:14
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-28 21:14:54
 * @FilePath: /cpptest/算法/面试/技能释放（不是dp，纯贪心）.cpp
 */


/*
一个战士，他在和一个物战斗，他有n个技能，使用第i个技能造成ai点伤害，在m个回合中，每个回合只能释放一种技能，每种技郎总共能痒放任意次数，
但不能连续释放同一个技能超过k次。请问他在这m回合中最多能造成多少伤害。
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    uint64_t n, m, k;
    cin >> n >> m >> k;

    vector<uint64_t> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    sort(a.begin(), a.end(),greater<uint64_t>());


    
    uint64_t ans2 = m /(k+1);
    uint64_t ans1 = m - ans2;

    uint64_t res = 1LL*a[0] * (ans1) + 1LL*a[1] * ans2;

    cout<<res;
    

    return 0;
}