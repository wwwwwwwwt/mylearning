<!--
 * @Author: zzzzztw
 * @Date: 2023-02-27 10:19:27
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-02-27 11:05:25
 * @FilePath: /cpptest/Effectivecpp/clause11.md
-->

# 实现安全的赋值运算符  

首先我们有一个Bitmap类和Wight类，Wight类中有一个指针指向Bitmap类并在堆上分配内存
```cpp
class Bitmap{...};
class Wight{
    ...
    private:
        Bitmap *pb;
};
```
当我们重载赋值运算符时可能会写出这样的代码 (错！)
```cpp
Wight& Wight::operator=(const Wight& rhs){
    delete pb;//删除当前指针
    pb = new Bitmap(*rhs.pb);//在堆上开一块新的内存，深拷贝
    return *this;
}
```

但是如果不小心写出了自赋值的代码(虽然一般不会)

```cpp
Wight w;
...
w = w;//自赋值
```
那么在赋值运算时，先把自己删了，会产生未定义行为，所以进行赋值时需要先检查自赋值行为。  
1.第一次修改  
```cpp
Wight& Wight::operator=(const Wight&rhs){
    if(this == &rhs){
        return *this;
    }
    delete pb;
    pb = new Bitmap(*rhs.pb);
    return this;
}
```
这个代码是否就安全了呢？

首先创建Bitmap可以大体分为两个步骤，1.开辟一块内存 2.调用Bitmap拷贝构造函数  
一旦这两个步骤中的任意一步抛出异常，因为我们已经先把this.pb删掉了，损坏了我们的Wight对象

我们进行第二次修改
```cpp
Wight& Wight::operator=(const Wight& rhs){
    Bitmap *temp = pb;
    pb = new Bitmap(*rhs.pb);
    delete temp;
    return *this;
}
```
乍一看我们没有检查自身赋值的情况，但我们产生自身赋值情况时也只是走一遍这个函数的流程，损耗了一些时间，而且自身赋值的情况很少见。  
但这种写法保证了new时抛出异常时，自身Wight对象的完整。