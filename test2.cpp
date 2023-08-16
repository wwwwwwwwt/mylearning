/*
 * @Author: zzzzztw
 * @Date: 2023-07-13 19:32:34
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-08-15 14:57:58
 * @FilePath: /myLearning/test2.cpp
 */
#include <bits/stdc++.h>
using namespace std;


int main(){
    
    try{
        int n;
        cin>>n;
        if(n == 5)__throw_invalid_argument("bad test");

    }catch(exception& e){
        cout<<e.what()<<endl;
    }
    return 0;
    
}