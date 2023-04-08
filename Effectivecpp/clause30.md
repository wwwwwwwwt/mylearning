<!--
 * @Author: zzzzztw
 * @Date: 2023-04-08 10:02:57
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-08 10:50:49
 * @FilePath: /cpptest/Effectivecpp/clause30.md
-->
# clause30. 理解inline

1. inline函数， 现代编译器基本不参考这个关键字来对函数是否进行展开。现在对函数的作用是防止多个文件同时引用一个文件中的一个函数，在主函数运行时会产生这个函数重复定义的错误。

2. inline静态变量：

```cpp
 //foo.h

struct foo{
    static int mfoo;
};

int foo::mfoo = 0; // 不加这一行编译不能通过,不会分配内存

//main.cpp


int main(){

    int num = foo::mfoo;

}

或者直接

struct foo{
    inline static int mfoo = 1; // 加了inline不需要再初始化
}


```