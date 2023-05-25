#include "test.h"
#include <iostream>
using namespace std;
using pii = pair<int,int>;
int main(){

    Vector<int>a;
    Vector<int>b;
    a.Push_back(1);
    a.Push_back(2);
    a.Push_back(3);
    a.Push_back(4);
    a.Push_back(5);
    b = a;
    a.POP_back();
    
   // cout<< a.size()<<" "<<b.size() <<endl;
    for(size_t i = 0; i< a.size(); i++){
        cout<<a[i]<<" ";
    }

    for(size_t i = 0; i< b.size(); i++){
        cout<<b[i]<<" ";
    }
    Vector<pii>c;
    c.Push_back({1,2});
    cout<<c[0].first <<" "<< c[0].second<<endl;
    return 0;
}