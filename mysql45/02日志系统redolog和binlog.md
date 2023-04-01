<!--
 * @Author: zzzzztw
 * @Date: 2023-03-28 10:54:52
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-28 16:03:25
 * @FilePath: /cpptest/mysql45/02日志系统redolog和binlog.md
-->
# 02.日志系统：一条SQL更新语句是如何执行的？

## redolog 日志模块（保证acid中的d 持久性）
### 特性
1. 物理日志，日志记录着对应着硬盘中哪个页中的哪个数据要改成什么值，更深层次执行器使用innodb引擎提供接口的操作
2. redo log可以保证数据库发生异常重启，之前提交的记录都不会丢失，crash-safe能力。
3. innodb独有，总共有四个文件，固定大小每个1G， 一共4个G。拥有write pos 和 checkpoint两个指针，当write pos 追上 checkpo时，redo log会强制刷盘，将一部分数据写入磁盘，checkpoints向前移动，为write pos 留下可写空间。
<center>

![](./img/02(1).png)

</center>

4. 落盘机制：落盘机制可以通过innodb_flush_log_at_trx_commit参数来控制：  （一般设置为1）
    1. 当设置为0的时候，事务提交时不会将log buffer中日志写入到os buffer，而是每秒写入os buffer并调用fsync()写入到log file on disk中。也就是说设置为0时是(大约)每秒刷新写入到磁盘中的，当系统崩溃，会丢失1秒钟的数据。（设置为0， 每秒都将redo log buffer中的数据刷到os buffer中再由操作系统调用fsync()进行刷盘） 
    2. 当设置为1的时候，事务每次提交都会将log buffer中的日志写入os buffer并调用fsync()刷到log file on disk中。这种方式即使系统崩溃也不会丢失任何数据，但是因为每次提交都写入磁盘，IO的性能较差。（设置为1，每次事务提交都将redo log buffer中的数据刷到os buffer中再由操作系统调用fsync()进行刷盘）
    3. 当设置为2的时候，每次提交都仅写入到os buffer，然后是每秒调用fsync()将os buffer中的日志写入到log file on disk。（）

<center>

![](img/02(2).png)

</center>


## binlog 日志模块
### 特性
1. 逻辑日志（server层实现），三种模式：statement， row ， mixed， statement记录着实际的sql语句， row记录哪条语句被修改的前后数据样子，mixed模式是statement和row的结合
2. 追加写，binlog是通过追加的方式进行写入，通过max_binlog_size调整每个binlog文件大小，写满了一个文件会新开一个文件来保存日志。相比于redo log 不会由于写满了而覆盖以前的日志。


## 一次更新数据update操作的流程
1. 先走一遍流程，连接器（管理连接，验证权限），分析器（词法分析：关键词对不对，语法分析：符不符合mysql语法，语义分析：语法对应的表存不存在等），优化器（先执行啥后执行啥）
2. 走到了执行器这一步，引擎使用树搜索找到对应的id主键，如果这个id本来就在内存中，直接返回给执行器，否则需要从磁盘中读入数据到内存。
3. 执行器拿到引擎给的数据，调用引擎给的接口更新行数据
4. 将新数据更新到内存中，然后将更新操作更新到redo log中，将redo log状态更新为prepare状态
5. 执行器生成这个更新操作的binlog文件
6. 执行器调用引擎的提交事务接口，并将redo log状态变为commit

<center>

![](img/02(3).png)

</center>

## 重要：两阶段提交
### 目的：让redo log 和 binlog两个日志之间的逻辑一致
1. 作用：一般redo log在数据库崩溃时进行回滚操作， binlog 通常用于回档到特定日期的一瞬间或用于主库通过发送binlog来保证与从库数据的同步
2. 崩溃时redo log prepare后崩溃，没有写binlog的情况：回滚时发现redo log 没commit 再看 binlog 没有这条记录，不会commit进磁盘，会继续向前回滚。备份时用binlog恢复，没有这条数据是一致的
3. 崩溃时redo log commit前崩溃，写完binlog的情况：发现没commit 但又prepare 和binlog，满足这两点完整会自动commit这条数据。 备份时用binlog恢复，有这个数据是一致的



## 之前写的，不一定准确一些补充
* redo log（innodb独有，固定大小四个文件每个1G，拥有write pos和 checkpoint,两个指针，在环形缓冲区中，当write pos 追上 checkpoint时，需要现将一部分记录更新到磁盘上，checkpoints向前擦除更新好的数据，为writepos腾出新的可写空间）: 物理日志，意思是对应着硬盘中哪个页中的那个数据要改成什么值，更深层次执行器使用innodb引擎提供接口的操作。
*  binlog（server层 全都有）:逻辑日志，意思是记录着所执行的sql语句以及row格式记录两条数据：修改前和修改后数据样子，记录的是逻辑的变化，bin log是追加写，写满一个文件会切换到下一个文件，并不会覆盖之前的。
*  两阶段提交：在更新数据时，先写入redo log，写完后redo log进入prepare阶段，在去写binlog内容， 写完binlog后再将事务提交，commit redo log。防止如果先写一个日志，再写一个日志，期间中间哪个步骤宕机，两份日志不一样的情况。
*  两阶段提交的例子：当一个数据库实例崩溃了，服务器重启，数据库按照redo log进行 "前滚" ，当发现最后一个事务只有prepare 没有commit时，就去看binlog，找到了完整的binlog就提交事务，找不到就回滚。
*  binlog是基于备份和binlog中的sql语句恢复数据库；redo log是为了保证事物提交后遇到crash，防止那些未写入磁盘、还保存在内存中的数据丢失。显然实际生产中如遇到突发情况，用redo log 可以更快恢复生产。两者的功能定位不同。此外，redolog是循环写的，不持久保存，binlog的“归档”这个功能，建从库的时候可以直接用binlog恢复数据，这个是redolog是不具备的。  