<!--
 * @Author: zzzzztw
 * @Date: 2023-02-28 12:01:09
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-02-28 12:36:29
 * @FilePath: /Effectivecpp/clause16.md
-->
# clause16. 成对使用 new 和 delete 时应采用相同形式
使用new来手动在堆上分配内存，使用delete来手动释放new的内存
1. 如假设有以下代码
```cpp
std::string *stringStr = new string;
std::string *stringArr = new string[100];

...
delete stringStr;
delete []stringArr;
```
我们在释放数组时，只告知编译器这是个数组，编译器怎么知道我们删除的数组的大小呢？  

答：下图是大部分编译器的实现(但不是规范),假设我们使用64位的系统，当编译器看到删除的是个数组类型时，会先往低地址移动8个字节，取出存放数组大小的值，依次对后面这个值数量的元素使用析构函数，最后将整个大的内存释放掉。
![](./resources/clause16.png)  

！！注意，有时可能会写出以下代码：
```cpp
typedef std::string AddressLines[4];
std::string *pel = new AddressLines;//new string[4]
delete pel;//error
delete []pel;//right
```

# clause17. 以独立语句将new对象放入智能指针
1. 现在最推荐使用shared_ptr<T>ptr = make_shared<T>(...)来创建智能指针
2. 为了避免后续语句抛出异常而导致new对象没有被delete,我们不应该将多个语句和new并列在一个函数执行，如下所示：
```cpp
int priority();
void process(std::shared_ptr<Wight>pw, int priority());
/*
如果编译器的执行顺序时先new->priority()->shared_ptr
当priority()抛出异常时，pw生成的new对象不会delete会导致内存泄漏
*/
process(std::shared_ptr<Wight>pw(new Wight),priority());
```
解决方法可以是：
```cpp
std::shared_ptr<Wight>pw(new Wight);
process(pw,prioity());
```