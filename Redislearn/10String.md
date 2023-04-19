<!--
 * @Author: zzzzztw
 * @Date: 2023-04-17 15:48:46
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-19 15:00:13
 * @FilePath: /myLearning/Redislearn/10String.md
-->
# 10. String 不能滥用，会造成很大内存消耗，导致产生大内存实例从而生成RDB的速度变慢，可以尝试使用压缩列表ziplist来代替，不过要谨慎元素最大长度和元素个数超过阈值压缩列表转换为哈希表。

