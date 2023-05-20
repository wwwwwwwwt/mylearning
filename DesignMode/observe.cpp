/*
 * @Author: zzzzztw
 * @Date: 2023-05-19 16:11:05
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-20 10:32:09
 * @FilePath: /myLearning/DesignMode/observe.cpp
 */
#include <iostream>
#include <string>
#include <list>
#include <memory>
using namespace std;

class Observer;

// 被观察者的抽象类，不同的被观察者之间可以复用, action为观察的动作。当被观察者状态变化时，使用notify通知当前链表中所有的观察者，执行其update操作
class BeObServed{
public:
    virtual ~BeObServed() { cout <<"调用了 BeObServed 析构函数 "<<endl;}
    string action;
    list<Observer* >oberservers; // 链表维护当前观察此对象的观察者
    virtual void Notify() = 0; // 通知观察者
    virtual void attach(Observer*) = 0; // 添加观察者进队列
    virtual void dettach(Observer*) = 0;  // 删除观察者进队列
};

class Observer{

protected:
    string name;
    BeObServed* sub;
public:
    Observer(string n, BeObServed* b):name(n), sub(b){};
    virtual ~Observer() { cout <<"调用了 Oberser 析构函数 "<<endl;};
    virtual void update() = 0; 
};



class firstObserver:public Observer{

public:
    firstObserver(string name, BeObServed* b):Observer(name, b){}
    ~firstObserver() {cout<<"调用了 firstOberserver 析构函数" <<endl;}
    void update();
};

void firstObserver::update(){
    cout<<"领导来了，快跑"<<" ";
    if(sub->action == "123"){
        cout<<name<<"跑走了";
    }
    printf("\n");
}

class Boss: public BeObServed{

public:
    
    Boss(){};
    ~Boss(){cout<<"调用了 Boss 析构函数"<<endl;}
    void attach(Observer*);
    void Notify();
    void dettach(Observer*);
};

void Boss::attach(Observer* o){
    oberservers.push_back(o);
}

void Boss::dettach(Observer* o){
    auto iter = oberservers.begin();
    while(iter != oberservers.end()){
        if((*iter) == o){
            oberservers.erase(iter);
            return;
        }
        iter++;
    }
}

void Boss::Notify(){
    auto iter = oberservers.begin();
    while(iter!=oberservers.end()){
        (*iter)->update();
        iter++;
    }
}




int main(){
    shared_ptr<Boss> b = make_shared<Boss>();
    b->action = "123";
    auto o1 = make_shared<firstObserver>("a", b.get());
    shared_ptr<firstObserver> o2 = make_shared<firstObserver>("b",b.get());
    b->attach(o1.get());
    b->attach(o2.get());
    b->Notify();
    return 0;
}