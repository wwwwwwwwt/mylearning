<!--
 * @Author: zzzzztw
 * @Date: 2023-04-19 13:27:41
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-19 13:37:10
 * @FilePath: /myLearning/memorypool/readme.md
-->
# 快速分配和释放内存：内存池

使用内存池理由：
1. 希望减少内存分配和释放的开销，更快的分配和释放
2. 希望减少内存分配和释放的开销，更少的总体内存使用  
3. 
内存池以下情况慎重使用  
1. 内存池可能导致操作系统更难回收你不需要的内存
2. 通用容器分配器已经做够快了
3. 内存池应该在单线程或者线程本地使用，否则性能可能会更差，不如通用内存分配器好

## 自定义内存池
* 定义内存池存储的数据对象的Node()包含数据段和一个指针，指向他的下一个内存块
* 定义内存块，内存块就是节点的数组和指向下一个内存块的指针，来把内存块串成一个链表