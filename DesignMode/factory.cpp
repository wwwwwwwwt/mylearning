/*
 * @Author: zzzzztw
 * @Date: 2023-05-16 09:17:42
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-16 10:05:30
 * @FilePath: /myLearning/DesignMode/factory.cpp
 */
#include <iostream>
using namespace std;


// 简单工厂实现
// 1.实现产品对象的抽象，作为纯虚函数，被子类继承。
// 2.产品对象继承抽象类
// 3.简单工厂类，有一个创造产品的方法接受一个字符串，根据字符串创建不同的实际产品
// 4.使用时创建工厂对象和父类对象的指针，使其指向工厂对象生产方法，生产传入字符串的类型 

/*
class Product{
public:
    Product() = default;
    virtual ~Product() = default;
    virtual void Show() = 0;
};

class ProductA :public Product{

public:
    ProductA() = default;
    ~ProductA() = default;

    void Show()override {
        cout << "Product A create!\n";
    }

};

class ProductB :public Product{

public:
    ProductB() = default;
    ~ProductB() = default;

    void Show()override {
        cout << "Product B create!\n";
    }

};

class Factory{
public:
    Factory() = default;
    ~Factory() = default;

    Product* CreateProduct(const string& str){
        if(str == "ProductA"){
            return new ProductA();
        }
        if(str == "ProductB"){
            return new ProductB();
        }
        return nullptr;
    }
};

int main(){
    Factory fa;
    Product *p = fa.CreateProduct("ProductA");
    delete p; //注意delete掉ProductA，否则内存泄漏
    p = fa.CreateProduct("ProductB");
    p->Show();
    delete p;
    return 0; 
}*/


//抽象工厂类
//将工厂也抽象出来，每个子类工厂对应生产对应产品
class Product{
public:
    Product() = default;
    virtual void Show() = 0;
    virtual ~Product() = default;
};

class Factory{
public:
    Factory() = default;
    ~Factory() = default;
    virtual Product* CreateFactory() = 0;
};

class ProductA: public Product{
public:
    ProductA() = default;
    ~ProductA() = default;
    void Show()override{
        cout<<"create Product A\n";
    }
};

class ProductB: public Product{
public:
    ProductB() = default;
    ~ProductB() = default;
    void Show()override{
        cout<<"create Product B\n";
    }
};

class FactoryA:public Factory{
public:
    Product* CreateFactory(){
        Product* p = nullptr;
        p = new ProductA();
        return p;
    }
};

class FactoryB: public Factory{
public:
    Product* CreateFactory(){
        Product* p = nullptr;
        p = new ProductB();
        return p;
    }
};

int main(){

    Product* A = nullptr;

    auto FaA = new FactoryA();

    A = FaA->CreateFactory();
    A->Show();
    
    delete A;

    auto Fab = new FactoryB();
    Product* B = nullptr;
    B = Fab->CreateFactory();
    B ->Show();
    delete B;
    return 0;

}