/*
 * @Author: zzzzztw
 * @Date: 2023-05-19 16:11:05
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-19 17:13:44
 * @FilePath: /myLearning/DesignMode/observe.cpp
 */
#include <iostream>
#include <string>
#include <list>

using namespace std;

class Observer;

// 被观察者的抽象类，不同的被观察者之间可以复用, action为观察的动作。当被观察者状态变化时，使用notify通知当前链表中所有的观察者，执行其update操作
class BeObServed{
public:
    ~BeObServed() = default;
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
    ~Observer() = default;
    virtual void update() = 0; 
};



class firstObserver:public Observer{

public:
    firstObserver(string name, BeObServed* b):Observer(name, b){}
    ~firstObserver() = default;
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
    ~Boss(){};
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
    BeObServed* b = new Boss();
    b->action = "123";
    Observer* o1 = new firstObserver("a", b);
    Observer* o2 = new firstObserver("b", b);
    b->attach(o1);
    b->attach(o2);
    b->Notify();
    return 0;
}