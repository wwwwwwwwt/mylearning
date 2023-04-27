/*
 * @Author: zzzzztw
 * @Date: 2023-04-26 22:02:46
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-26 22:29:27
 * @FilePath: /myLearning/算法/面试/批量初始化次数.cpp
 */

/*
拓扑排序 + 层序遍历

(1)第1行只有一个数字,表示模块总数N。
(2)随后的N行依次表示模块1到N的依赖数据。每行的第1个数据表示依赖的模块数量(不会超过N)，之后的数字表示当前模块依赖的模块ID序列,该序列不会重复出现相同的数字,模块ID的取值一定在[1,N]之内。
(3)模块总数N取值范围 1<=N<=1000
(4)每一行里面的数字按1个空格分隔
输出
输出"批量初始化次数”,若有循环依赖无法完成初始化，则输出-1

5
3 2 3 4
1 5
1 5
1 5
0
共5个模块。模块1依赖模块2、3、4:模块2依赖模块5模块3依赖模块5模块4依赖模块5:模块5没有依赖任何模块批量初始化顺序为 5 -> 2，3,4 ->1共需"批量初始化”3次
*/

#include <bits/stdc++.h>
using namespace std;

const int N = 1010;
vector<int>g[N];
int d[N];
int n;
int main(){
    cin>>n;
    for(int i = 1;i<=n;i++){
        int num = 0;
        cin>>num;
        d[i] = num;
        for(int j = 0; j< num; j++){
            int t;
            cin>>t;
            g[t].push_back(i);
        }
    }

    queue<int>q;
    for(int i= 1; i<=n;i++){
        if(d[i] == 0)q.push(i);
    }
    if(q.size() == 0)cout<< -1 <<endl;
    int res = 0;
    while(q.size()){
        int n = q.size();
        for(int i = 0; i< n;i++){
            auto t = q.front();
            q.pop();
            for(auto &i :g[t]){
                d[i]--;
                if(d[i] == 0){
                    q.push(i);
                }
            }
        }
        res++;
    }
    for(int i= 1; i<=n;i++){
        if(d[i] != 0){
            cout<<-1<<endl;
            return 0;
        }
    }

    cout<<res<<endl;
    return 0;
}