<!--
 * @Author: zzzzztw
 * @Date: 2023-04-21 17:01:36
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-21 17:06:14
 * @FilePath: /myLearning/Redislearn/15redis删除数据后，为什么内存占用率还是很高.md
-->
# 15 删除数据后，为什么内存占用率还是很高？

### 两个大原因：1. reids释放的空间不会立即返回给操作系统而是由内存管理器进行处理 2. 释放的空间可能有很多内存碎片，没法继续使用，浪费空间也影响运行效率。