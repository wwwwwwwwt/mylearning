/*
 * @Author: zzzzztw
 * @Date: 2023-05-19 17:16:07
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-20 10:49:22
 * @FilePath: /myLearning/DesignMode/decorate.cpp
 */
#include <memory>
#include <iostream>
using namespace std;

class tansform{ // 抽象类，被装饰器类继承
public:
    virtual void move() = 0;
};

class Car : public tansform{
public:
    Car(){
        cout<<"变形金刚是一辆车！ "<<endl;
    }
    void move(){
        cout<<" 在陆地上移动"<<endl;
    }
};



//装饰类，用于抽象类和具体装饰之间的桥梁，具体装饰类继承这个装饰类，并提供新的功能函数
class changer : public tansform{
public:
    changer(shared_ptr<tansform>t):tans(t){};
    void move(){
        tans->move();
    }
private:
    shared_ptr<tansform> tans;
};

// 具体装饰类


class Robot: public changer{
public:
    Robot(shared_ptr<tansform>t):changer(t){};
    void say(){
        cout<<"我是个机器人"<<endl;
    }
};


int main(){
    shared_ptr<tansform>t = make_shared<Car>();
    t->move();
    shared_ptr<Robot>r = make_shared<Robot>(t);
    r->say();
    return 0;
}

