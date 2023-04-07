<!--
 * @Author: zzzzztw
 * @Date: 2023-04-05 22:09:58
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-07 11:38:42
 * @FilePath: /cpptest/Effectivecpp/clause27.md
-->
# clause27. 强制转换变量类型

## c++风格转换

```cpp

1. const_cast<T>(expression) 用于带有常量的指针，将const去除，或增加const

2. dynamic_cast<T>(expression) 
用于父类指针向下转换为子类指针，安全向下转型；
代价很高，其他三个转换都发生在编译时，只有这个发生在运行时

3. reinterpret_cast<T>(expression)
主要用于指针之间，或指针和整形之间转换，一般不使用，高危操作，从底层对数据类型重新解释，
可以将整形转换为指针，将指针转换为数组，将指针和引用互相转换。

4. static_cast<T>(expression)
用于整形之间转换，明确指出类型转换，没有动态类型检查，所以上行转换安全（派生类转换为基类安全），下行转换不安全（基类转换为派生类）
主要用于执行非多态的转换操作。

例如：
void dosome(const wiget& w);
dosome(wiget(15));或dosome(static_cast<wiget>(15));

int x,y;
....
double res = static_cast<double>(x)/y;
```

## static_cast 的注意点

1. 虚函数中会出现基类指针和派生类指针指向同一个对象但地址不一样的情况（派生类指针会比基类指针向前偏移8个字节，为虚函数表）

```cpp
#include <iostream>

class Base{
public:
    int a;
};

class Derive:public Base{
public:
    double c{};
    virtual void bar;
};


int main(){
    Derive d;
    Base* base = static_cast<Base*>&d;// 隐式转换也一样
    Derived * derive = &d;
    printf("base：%p, derive: %p\n", base,derive);
    if(base == derived){
        cout<<"==\n";
    }
    else{
        cout<<"!=\n";
    }
    return 0;

    输出：
    base： 0x.....48, derive:0x......40
}
```
* 由于虚函数表的存在，derive\*会指向vtable，8个字节，之后才是base\* 指向对象的开始，变量a，然后字节对齐，vtable 8 字节，a 4字节，对其4字节，c 8字节 一共24字节。
## dynamic_cast 
1. 开销很大，尽量少用不用在循环内部，可以寻找提供虚函数多态的方式代替。