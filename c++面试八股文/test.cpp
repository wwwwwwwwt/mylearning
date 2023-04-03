#include <bits/stdc++.h>
using namespace std;

void func(int *p , int *t){
    p = t;
}

int main(){

    int* p = new int[15];
    int* t = new int[23];

    cout<<p<<"     "<<t<<endl;

    func(p,t);

    cout<<p<<"     "<<t<<endl;

    delete[] t;
    delete[] p;

    return 0;
}