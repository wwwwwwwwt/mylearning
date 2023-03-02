<!--
 * @Author: zzzzztw
 * @Date: 2023-03-02 09:18:30
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-02 10:43:03
 * @FilePath: /cpptest/Effectivecpp/clause20.md
-->
# 使用const引用传递优于按值传递
1. 有以下代码：
```cpp
class Person{
public:
    Person();
    ~Person();
private:
    string name_;
    string address_;
};
class student : public Person{
public:
    student();
    ~student();
private:
    string schoolName;
    string schoolAddress;
};
bool validStudent(student s);

student p;
bool flag = validStudent(p);
```
当我们把p传入validStudent函数时，首先会生成一个临时变量拷贝一次p,在调用复制构造函数将对应的string成员变量拷贝
这最少将拷贝五次，并且离开函数时，这个临时变量将销毁，所以为了避免不必要的开销，推荐改为这样
```cpp
bool validStudent(const student& s);
```
这样还有一个好处是，当对象以指针或引用的形式时，才能表现出多态，按值传递时调用方法时始终表现为基类方法
补充：
    在有多态关系/虚函数时，Base *ba = new dervide(), ba调用虚方法时会使用指针指向对象的虚方法  
    Base ba = new dervide(), ba调用虚方法始终是基类的虚方法  
    如果按值传递 dervive da = ba, 将产生截断切片行为，dervide* da = new base()//报错，不能将基类绑定到一个派生类上

2. 总结：
按常引用传递参数将避免多次复制，避免多态情况下切片行为  
例外：使用内置类型：int,double等按值传递将占用较小内存，按指针字节比按值消耗大