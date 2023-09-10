/*
 * @Author: zzzzztw
 * @Date: 2023-09-06 13:22:51
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-09-08 19:40:57
 * @FilePath: /myLearning/testnew.cpp
 */
#include <bits/stdc++.h>
using namespace std;

int test(int x){
    return (x == 1) ? 1 :(x + test(x - 1));
}
void func(int p[][3]){
    cout<<1<<endl;
}

struct A{
    void foo(){
        cout<<"foo"<<endl;   
     }
    virtual void bar(){
        cout<<"bar"<<endl;
    }
    A(){
        bar();
    }
};

struct B: A
{
    void  foo(){
        cout<<"b_bar"<<endl;
    }
    void bar(){
        cout<<"b_bar"<<endl;
    }
};


int main(){

    A *p = new B;
    p->foo();
    p->bar();

    cout<<test(12)<<endl;
    int a[2][3] = {0};
    func(a);

    int n;
    cin>>n;
    string str;
    while(n){
        if(n & 1)str += "1";
        else  str += '0';
        n >>= 1;
    }
    reverse(str.begin(), str.end());
    cout<<str<<endl;
    return 0;

    
}