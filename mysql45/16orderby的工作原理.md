<!--
 * @Author: zzzzztw
 * @Date: 2023-04-20 15:15:01
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-20 16:33:41
 * @FilePath: /myLearning/mysql45/16orderby的工作原理.md
-->
# order by 是怎么工作的

* 假设有个市民表

```sql

create table t (

    'id' int(11) NOT null,
    'city'varchar(16)not null,
    'name'varchar(16)not null
    'age' int(11) not null,
    'addr' varchar（128）not null,
    PRIMARY KEY ('id'),
    KEY 'city'('city')
)ENGINE=InnoDB;



```


1. 假设要查市民表中所有杭州人的名字，并且按照名字顺序返回前1000个人名字

* limit count 取前count行
* limit offset，count 跳过 offset行再取前count行

```sql

select city, name, age from t where city='杭州' order by name limit 1000;


```

临时表：用到临时表时，如果查询如果字段太多的话，超过内存临时表的最大限制（16M）就会生成磁盘临时表否则时内存临时表，用到内存临时表时会优先走rowid算法，因为只排序字段和主键id，用到的字段少优先被考虑

由于我们再city上加了索引,所以这句话的流程为：也被称为全字段排序   
* 初始化一个用于排序的内存，sort buffer
* 根据索引，查到对应id，回表查询整个字段，取出city，name ，age三个字段，加入到sort buffer。
* sort buffer内按name进行排序
* 返回前1000的结果集

<center>

![](img/16(1).png)

</center>


1. 如果字段太多的话，会走rowid排序，就是指取主键id和排序字段两个字段：

* 初始化排序内存
* 将name和 主键id放入sort buffer进行排序
* 将排好序的name 按照id回表取出字段city name age 返回结果集

因为多回表了一次所以效率差

3. 两种对比，mysql认为内存足够大时，优先全字段排序，减少磁盘io。

### 并不是所有的order by都需要排序，如果这个字段已经排好序了就不需要排序了
1. 这个例子中，如果创建联合索引 

```sql

alter table t add index city_name (city, name);

```

此时city和name都排序好了，那么将不需要排序，直接取前1000个name索引下的id，再回表取出相应字段即可

2. 如果创建覆盖索引（需要查询的字段都被覆盖叫做覆盖索引），将直接不回表，直接取前1000个结果

```sql
alter table t add index city_name_age (city,name, age);

```

