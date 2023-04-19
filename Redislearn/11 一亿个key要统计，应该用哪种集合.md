<!--
 * @Author: zzzzztw
 * @Date: 2023-04-18 21:53:39
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-19 15:43:20
 * @FilePath: /myLearning/Redislearn/11. 一亿个key要统计，应该用哪种集合.md
-->
# 大量数据要统计，需要什么数据类型？

## 四种常见的集合统计模式：聚合统计，排序统计，二值状态统计，基数统计

1. 聚合统计（使用set，但复杂度比较高，数据量比较大时可能会导致实例阻塞，建议使用一个从库专门做统计）：统计多个集合的共有元素（交集统计）；把两个集合相比，统计其中一个集合独有的元素（差集统计）；统计多个集合的所有元素（并集统计）。

2. 排序统计（list， soted set）：list是按照元素进入list的顺序进行排序的，sorted set可以根据元素的权重来排序，分页排序时候list会出现问题，推荐sorted set

3. 二值状态统计（bitmap，其底层时string）：指集合元素的取值只有0和1两种，使用setbit和getbit来设置和检查这个位置是不是1，使用bitcount来统计多少个1

4. 基数统计（set， hash， hyperlonglong（数据特别多时使用，固定空间大小，节约空间））