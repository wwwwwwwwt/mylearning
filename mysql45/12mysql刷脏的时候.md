<!--
 * @Author: zzzzztw
 * @Date: 2023-04-17 16:44:57
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-17 21:05:29
 * @FilePath: /cpptest/mysql45/12mysql刷脏的时候.md
-->
# mysql为什么会抖一下变慢

1. mysql采用WAL机制，即write ahead log，执行一条sql语句时，更新完内存数据 + 写完redo log后就返回给客户端显示本次成功。
2. 但实际磁盘中的数据并没有被更新掉，内存中的数据和磁盘中的数据不一样，我们称这个数据页为脏页。当内存中的数据刷页更新磁盘中的数据页后，这个数据页叫干净页。脏页和干净页都再内存中。不在内存中的数据页表示没有被动过，没有脏的可能。
3. 平时执行很快的更新语句，实际上就是在写内存和日志，偶尔缓慢的那以下，就可能是再刷脏页进磁盘。

## 刷脏页的时机

1. redo log环形区域写满了，write pos 快追上了 check point。 此时，系统必须停下所有操作，进行刷脏，将上面redo log 对应的脏页刷进磁盘，checkpoint才能向后移动。

2. 内存满了，需要根据LRU进行页的淘汰，发现淘汰的是脏页时，进行刷脏。

3. 系统空闲时，后台线程会自动进行刷脏。

4. 正常关闭时，会将内存所有的脏页flush到磁盘，下次启动时直接读取数据，启动速度快。


## 刷脏页的控制策略

 ```
 fio -filename=$filename -direct=1 -iodepth 1 -thread -rw=randrw -ioengine=psync -bs=16k -size=500M -numjobs=10 -runtime=10 -group_reporting -name=mytest 

 ```

1. 根据主机的io能力匹配相应的```innodb_io_capacity```