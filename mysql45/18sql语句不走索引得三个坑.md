<!--
 * @Author: zzzzztw
 * @Date: 2023-04-20 19:20:52
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-20 19:43:51
 * @FilePath: /myLearning/mysql45/18sql语句不走索引得三个坑.md
-->
#  18. 为什么这些SQL语句逻辑相同，性能却差异巨大？

## 主旨：对索引字段进行函数操作会导致索引失效，包括直接函数操作/ 隐式转换 / 字符集utf8 转换为utf8mb4等‘

1. 直接对条件字段进行函数操作
   
```sql

mysql> CREATE TABLE `tradelog` (
  `id` int(11) NOT NULL,
  `tradeid` varchar(32) DEFAULT NULL,
  `operator` int(11) DEFAULT NULL,
  `t_modified` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `tradeid` (`tradeid`),
  KEY `t_modified` (`t_modified`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

```

* 假设统计所有年份中7月份的交易总数

```sql

mysql > select count(*) from tradelog where month(t_modified) = 7;

```

这就是典型的对索引字段使用了函数操作,会导致索引失效，这是mysql得规定，导致走全局扫描。正确操作应该如下：

```sql


mysql> select count(*) from tradelog where
    -> (t_modified >= '2016-7-1' and t_modified<'2016-8-1') or
    -> (t_modified >= '2017-7-1' and t_modified<'2017-8-1') or 
    -> (t_modified >= '2018-7-1' and t_modified<'2018-8-1');

```


2. 隐式类型转换

```sql

mysql> select * from tradelog where tradeid=110717;

```

注意：tradeid是varchar类型，等号右边是整形，导致varchar类型自动进行隐式类型转换函数，字符串类型与整形比较，默认字符串类型转换为整型。实际上是下面这个语句，同样对索引字段进行了函数操作，使索引失效。

```sql

mysql> select * from tradelog where  CAST(tradid AS signed int) = 110717;


```

如果下面这样，整型 = 字符串， 索引不会失效，因为是字符串转换为整型，不会导致全表扫描

```sql

select * from tradelog where id="83126";

```


3. 隐式字符编码转换，两个字符集不同的表进行联表查询，会使字符字段进行转换如utf8 转换为 utf8mb4 导致字段索引失效。