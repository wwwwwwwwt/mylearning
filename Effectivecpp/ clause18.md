<!--
 * @Author: zzzzztw
 * @Date: 2023-03-01 11:03:35
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-01 13:20:04
 * @FilePath: /cpptest/Effectivecpp/ clause18.md
-->
# clause18. 让接口容易被正确使用，不易被误用
## 整体思想是编写可以让编译器提示我们代码错误的发生的接口（以自定义类型 + explicit关键字传参，智能指针RAII）
1. 第一个例子：
```cpp
    class Date{
    public:
        Date(int month, int day, int year);
        ...
    }
```
当使用者错误的使用了接口时，如传入错误参数：
```cpp
    Date d(30, 3, 1995)//错误，月份和日期写反了
    Date d(3,40,1995)//一个月没30天
```
所以我们在定义接口时应尽可能的由编译器检查错误，比如将参数变为类的对象传入,这样可以避免参数顺序不对：
```cpp
struct day{
    int val;
    explicit day(int  v): val(v):{}
};
struct Month{explicit Month()...};
struct Year{explicit Year()...};
Date(const day& m, const month& n, const year& y);
Date(3,2,1996); // error, 因为结构体构造函数explicit 禁止类型隐式转换
Date d(day(20), month(1), year(1996));
```
再来看天数的约束，我们可以在类中定义一些静态的方法或枚举类，调用指定的月份
```cpp
class Month{
public:
    static Month Jan(){return Month(1);}
    ...
private:
    int val;
    explicit Month(int v):val(v){}
}
```
可以看出，这种情景整体思想就是使用```explicit```将参数包装成对象，再以对象的形式传入函数，预防错误传参。

2. 第二个例子：
```cpp
if(a * b = c)... //错误，应该为== 不是 = 
```
如果a，b是内建类型时，编译器会直接报错，因为企图对右值进行赋值，但如果a,b是我们自定义类型的话，并且重载了*，相当于将c赋值给了 一个临时变量，这个if语句将一直为true, 为了避免这种情况，我们应在自定义类型时，多使用const修饰返回值。
3. 第三个例子：
```cpp
Investment * createInvestment();
如单例模式，我们可能会需要手动释放资源，产生内存泄漏风险，可以将其放在智能指针 + 自定义删除器来使用如下：
#include <bits/stdc++.h>
using namespace std;
// 我们就需要用到shared_ptr可以指定删除器的特点，自定义删除器。
class Singleton
{
private:
    Singleton()
    {
        cout << "创建了一个单例对象" << endl;
    }
    Singleton(const Singleton &);
    Singleton &operator=(const Singleton &);
    ~Singleton()
    {
        //析构函数我们也需要声明成private的
        //因为我们想要这个实例在程序运行的整个过程中都存在
        //所以我们不允许实例自己主动调用析构函数释放对象
        cout << "销毁了一个单例对象" << endl;
    }
    static void DestroyInstance(Singleton *); //自定义一个释放实例的函数

    static shared_ptr<Singleton> instance; //这是我们的单例对象,它是一个类对象的指针
public:
    static shared_ptr<Singleton> getInstance();
};

//下面这个静态成员变量在类加载的时候就已经初始化好了
shared_ptr<Singleton> Singleton::instance(new Singleton(), Singleton::DestroyInstance);

shared_ptr<Singleton> Singleton::getInstance()
{
    return instance;
}
void Singleton::DestroyInstance(Singleton *a)
{
    // cout << "在自定义函数中释放实例" << endl;
    // delete instance;
    //   cout << a << endl;
    delete a; // 释放单例对象
}

int main()
{
    cout << "Now we get the instance" << endl;
    shared_ptr<Singleton> instance1 = Singleton::getInstance();
    cout << instance1 << endl;
    shared_ptr<Singleton> instance2 = Singleton::getInstance();
    cout << instance1 << endl;
    shared_ptr<Singleton> instance3 = Singleton::getInstance();
    cout << instance1 << endl;
    cout << "Now we destroy the instance" << endl;
    return 0;
}
```
