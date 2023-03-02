<!--
 * @Author: zzzzztw
 * @Date: 2023-03-02 09:10:46
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-02 09:18:01
 * @FilePath: /cpptest/Effectivecpp/clause19.md
-->
# clause19. 设计class的十二个问题
1. 怎么创造你的对象和销毁对象
2. 怎么初始化你的对象
3. 拷贝/移动函数怎么实现
4. 怎么限定你类中某些值
5. 怎么设计类的虚函数让你的类可继承，类似于析构函数虚函数
6. 设计的类型是否支持转换，explicit
7. 设计类型需要哪些操作->成员函数
8. 哪些函数应该被禁止 = delete
9. 考虑成员函数/成员变量的访问限定符public/private/protect
10. 实现向外的承诺，线程安全/异常安全
11. 模板类
12. 真的是否需要实现一个新的类。