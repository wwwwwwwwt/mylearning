<!--
 * @Author: zzzzztw
 * @Date: 2023-04-03 14:50:17
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-03 16:25:11
 * @FilePath: /cpptest/c++面试八股文/c++基础.md
-->
# 1.为什么要用智能指针
答：
* 为了防止使用原生的new后忘记delete而造成内存泄漏的问题，其原理是RAII，即在构造函数时分配资源，在析构函数时释放资源。
* 常用接口
```cpp
T* get() //获得原生指针
T& operator*()//重载取值
T* operator->()//重载箭头取取成员函数 相当于* .
T* release()//清空当前指针，返回清空前指向的地址

```

* 常用智能指针
* 1. auto_ptr 已经废弃，原因是它支持拷贝构造和拷贝赋值，在使用时不经意间会导致对象被变为野指针
* 2. unique_ptr auto_ptr的改进，显示的声明移动构造和移动赋值，相当于删除了拷贝构造和拷贝赋值，在使用时需要显示的声明右值引用才行，表明我们已经知道这样做会导致对象所有权的转移
* shared_ptr 引用了引用计数，内部底层由T* ptr的数据指针和一个计数类shared_count类的指针（管理着引用计数和弱指针的一些）组成，只有当引用计数归零时才会释放数据指针。
* weak_ptr 为了解决shared_ptr互相引用而导致计数永远不可能为0的情况，因为他的构造函数和析构函数不会造成计数的增加或减少，只能由shared_ptr 和weak_ptr转换得到

# 2.c++中内存分配的情况
1. 栈：存放局部变量和函数参数，由编译器管理分配和回收
2. 堆：自己管理 使用new / delete 进行分配和回收，但可能出现内存泄露
3. 全局/静态存储区：分为已初始化和未初始化的区域，分别存放已初始化和未初始化的全局变量和静态变量
4. 常量存储区：存放常量或字面值常量
5. 代码区：存放程序的二进制代码
```cpp
#include <iostream>
using namespace std;
/*
说明：C++ 中不再区分初始化和未初始化的全局变量、静态变量的存储区，如果非要区分下述程序标注在了括号中
*/
int g_var = 0; // g_var 在全局区（.data 段）
char *gp_var;  // gp_var 在全局区（.bss 段）

int main()
{
    int var;                    // var 在栈区
    char *p_var;                // p_var 在栈区
    char arr[] = "abc";         // arr 为数组变量，存储在栈区；"abc"为字符串常量，存储在常量区
    char *p_var1 = "123456";    // p_var1 在栈区；"123456"为字符串常量，存储在常量区
    static int s_var = 0;       // s_var 为静态变量，存在静态存储区（.data 段）
    p_var = (char *)malloc(10); // 分配得来的 10 个字节的区域在堆区
    free(p_var);
    return 0;
}
```


# 3.c++函数中按指针传参和按引用传参
1. 不同点：引用传参为间接寻址，直接操作本体；按指针传参实际上还是传入的本体指针的副本，在函数体内对指针的操作不影响原指针，除非传入指针的指针或指针引用。
```cpp

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

输出：
0x5571b0e41eb0     0x5571b0e41f00
0x5571b0e41eb0     0x5571b0e41f00

改为引用的话就改变了地址，但delete []需要修改,因为指向同一块内存。两次delete[]同一块内存未定义行为报错
0x560bfcd54eb0     0x560bfcd54f00
0x560bfcd54f00     0x560bfcd54f00
```

# 4. c++ static 关键字和 const 关键字
## static
* 修饰局部变量：延长其生命周期，将其放在静态区中，与程序的生命周期相同，但作用域没变，作用域还是限定在语句块
* 修饰全局变量或函数： 改变作用域，由全工程可见变为本文件可见
* 修饰类：对类中某个函数使用static 表示这个函数不属于任何这个类的对象，而是整个类共用；如果类中某个变量进行static修饰，表示该变量为所有对象共用一个，存储空间只存在一个副本，可以通过类的对象调用，静态非常量数据成员只能在类外定义和初始化，在类内只能声明。
* static 类对象必须要在类外进⾏初始化，static 修饰的变ᰁ先于对象存在，所以 static 修饰的变ᰁ要在类外初
始化；
* 由于 static 修饰的类成员属于类，不属于对象，因此 static 类成员函数是没有 this 指针，this 指针是指向本
对象的指针，正因为没有 this 指针，所以 static 类成员函数不能访问⾮ static 的类成员，只能访问 static修饰
的类成员；
* static 成员函数不能被 virtual 修饰，static 成员不属于任何对象或实例，所以加上 virtual 没有任何实际意
义；静态成员函数没有 this 指针，虚函数的实现是为每⼀个对象分配⼀个 vptr 指针，⽽ vptr 是通过 this 指
针调⽤的，所以不能为 virtual；虚函数的调⽤关系，this->vptr->ctable->virtual function
```cpp

class A
{  
private:
    static int count ; // 类内声明
};

int A::count = 0 ; // 类外初始化，不必再加static关键字
---------------------------------------------------
class A
{  
private:
    static const int count = 0; // 静态常量成员可以在类内初始化
};

```

## const
* const 修饰基本类型数据类型：基本数据类型，修饰符 const 可以⽤在类型说明符前，也可以⽤在类型说明符后，其结果是⼀样的。
* const 修饰指针变量和引用变量，const 在\*左侧表示指针指向的值不能变， const在\*右侧表示指针指向的地址不能变
* const 在类中， const成员变量只能使用构造函数初始化列表的方式进行初始化。

# 5. C和C++区别
* c++新增关键字等：new delete auto explicit
* c++有函数重载的的概念（静态多态），c语言不能出现同名函数， c++有虚函数概念（动态多态），用来实现多态。
* c语言中strcut中不可以有成员函数，只能有成员变量， c++中都可以有，并且c++有class类这个概念，与strcut不同为struct默认pulic class默认private
* c语言没有模板函数概念，如果实现必须用到宏，c++有模板函数概念，提高了函数的复用
* c++提供stl标准库和容器