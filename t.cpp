#include <iostream>
#include <vector>
#include <memory>
#include <functional>
using namespace std;



/*
class base
{
public:
    virtual void print(){
        cout<<"base"<<endl;
    }
    virtual void get() = 0;
     ~base(){
        cout<<"delete base"<<endl;
    }
    double a;
    double b;
};
class base2 : public base
{
public:
    void print(){
        cout<<"base 2"<<endl;
    }
    virtual void ccc(){
        cout<<"cccc"<<endl;
    }
};

class  dervive :public base
{
public:
    ~dervive(){
        cout<<" delete dervive"<<endl;
    }
    void get() override{
        cout<<"get"<<endl;
    }
};

int main(){
    dervive e;
    base *b = &e;
    b->print();

    auto *c = b;
    auto &d = e;
    cout<<sizeof(*c)<<" "<<sizeof(c)<<" "<<sizeof(d)<<endl;
    cout<<"-----------"<<endl;
    vector<int>a = {0,1,2,3,4,5,6};
    auto it = a.begin();
    for(;it!=a.end();){
        cout<<*it<<endl;
        it = a.erase(it);
    }

    shared_ptr<base>ptr= make_shared<dervive>();
    ptr->get();
    weak_ptr<base>wk = ptr;
    auto k = wk.lock();
    k->print();
    cout<<a.size()<<" "<<a.capacity()<<endl;

    cout<<a.size()<<" "<<a.capacity()<<endl;
    return 0;
}*/

#include <bits/stdc++.h>
using namespace std;

extern "C"
__attribute__((constructor))void func(){
    printf("%s\n", "12345");
}

struct ListNode{
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr){};
    ListNode(int v):val(v), next(nullptr){};
};

void build(vector<int>&a){
    ListNode* dummyhead = new ListNode(0);
    ListNode *cur = dummyhead;
    for(int i = 0; i < a.size(); i++){
        ListNode* node = new  ListNode(a[i]);
        cur->next = node;
        cur = cur->next;
    }

    unordered_map<int ,int>mp;
    cur = dummyhead;
    while(cur && cur->next){
        mp[cur->next->val]++;
        cur = cur->next;
    }

    cur = dummyhead;
    while(cur && cur->next){
        while(cur && cur->next && mp[cur->next->val] > 1){
            ListNode *node = cur->next;
            cur->next = cur->next->next;  
            delete node;
        }
        cur = cur->next;
    }

    cur = dummyhead;
    while(cur->next){
        cout<<cur->next->val<<" ";
        cur = cur->next;
    }
    cout<<endl;
    cur = dummyhead;
    while(cur && cur->next){
        ListNode* node = cur->next;
        cur = cur->next->next;
        delete node;
    }
    delete dummyhead;

}

void func3(int a, int b){
    cout<<&a<<" "<<&b<<endl;
}
union un{
    int a;
    int b;
    int c;
};

class base{
public:
    base(){
        a = 1234;
    }
    virtual ~base(){

    };
    int a;
};

class dervive:public base
{
public:
    dervive(){
        ba = new base();
    }
    ~dervive(){
        delete ba;
    }
 
 double c = 3.5;
 base* ba;
};




int main(){

    atexit([](){
        cout<<"bye"<<endl;
    });
    atexit([](){
        cout<<"bye2"<<endl;
    });
    cout<<"kkkk"<<endl;

    int a = 0, b = 0;

    auto res = [a, &b]()mutable->void{
        a = 1, b = 2;
    };
    res();
    func3(a, b);
    cout<<a <<" "<<b<<endl;

    int e[2][3] = {{0,1,2},{2,3,4}};
    char f[] = "123";
    cout<<sizeof(f)<<" "<<strlen(f)<<endl;
    cout<<sizeof(e[0])<<" "<<sizeof(e[0]) / sizeof(e[0][0])<<endl;
    cout<<e[1]<<" "<<&e<<endl;

    int test[] = {1,2,3,4,5};
    int *test2 = new int[5];
    cout<<sizeof(test)<<" "<<sizeof(test2)<<endl;
    un testun;
    cout<<"testun: "<<sizeof(testun)<<endl;
    dervive *der = new dervive();
    cout<<sizeof(der)<<endl;
    base* testdd = dynamic_cast<base*>(der);
    cout<<testdd->a<<endl;


    vector<int>list = {1,1,2,2,3,4,5,6,7};

    build(list);

    return 0;
}

__attribute__((destructor))void func2(){
    printf("%s\n", "23456");
}