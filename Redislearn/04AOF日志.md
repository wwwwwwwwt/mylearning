<!--
 * @Author: zzzzztw
 * @Date: 2023-03-17 09:19:33
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-17 14:34:18
 * @FilePath: /cpptest/Redislearn/04AOF日志.md
-->
# AOF日志：宕机了，Redis如何避免数据丢失？

1. redis采用了两种持久化的机制来防止服务器宕机，内存中的数据全部丢失
* AOF日志（append on file）和RDB快照
## AOF日志是如何实现的？
### 1. 先来回顾一下mysql中所学的WAL日志，也就是write ahead log 写前日志，意思是在实际写数据前，先把数据写进日志中，以便故障恢复。
* redo log（innodb独有，固定大小四个文件每个1G，拥有write pos和 checkpoint,两个指针，在环形缓冲区中，当write pos 追上 checkpoint时，需要现将一部分记录更新到磁盘上，checkpoints向前擦除更新好的数据，为writepos腾出新的可写空间）: 物理日志，意思是对应着硬盘中哪个页中的那个数据要改成什么值，更深层次执行器使用innodb引擎提供接口的操作。
*  binlog（server层 全都有）:逻辑日志，意思是记录着所执行的sql语句以及row格式记录两条数据：修改前和修改后数据样子，记录的是逻辑的变化
*  两阶段提交：在更新数据时，先写入redo log，写完后redo log进入prepare阶段，在去写binlog内容， 写完binlog后再将事务提交，commit redo log。防止如果先写一个日志，再写一个日志，期间中间哪个步骤宕机，两份日志不一样的情况。
*  两阶段提交的例子：当一个数据库实例崩溃了，服务器重启，数据库按照redo log进行 "前滚" ，当发现最后一个事务只有prepare 没有commit时，就去看binlog，找到了完整的binlog就提交事务，找不到就回滚。
*  binlog是基于备份和binlog中的sql语句恢复数据库；redo log是为了保证事物提交后遇到crash，防止那些未写入磁盘、还保存在内存中的数据丢失。显然实际生产中如遇到突发情况，用redo log 可以更快恢复生产。两者的功能定位不同。此外，redolog是循环写的，不持久保存，binlog的“归档”这个功能，建从库的时候可以直接用binlog恢复数据，这个是redolog是不具备的。  
### 2. AOF与上文所提正好相反，它第一步先执行命令写内存，第二步再进行写日志。
* 为什么先执行命令再写日志呢，传统wal日志如redo log记录的是修改后数据要改成什么值，如上；AOF记录的是redis收到的每条命令，很像binlog statement模式下记录sql语句。  
<center>

  ![](./img/04(1).png)

</center>

* 但是写入的语句不一定会成功（没有mysql那样的语法分析，词法分析），为了避免检查语句的开销，写后日志模式就可以先让系统执行，只有执行成功的命令才会被记录下来。还有一个好处就是，命令执行后才记录日志，所以不会阻塞当前的写操作。但无法保证命令执行和写日志一致性，与redis定位有关，是一种非高一致性存储，主要高性能为目标。
### 3. 两个潜在风险
* 如果刚执行完一个命令，没记录日志就宕机了，那么这个命令和对应数据日志就有丢失的风险
* 在写回策略为always时，由主线程写回磁盘，如果当前日志很大的话，写回磁盘压力大，对下一次操作有影响。
* 所以怎么选择写回时机策略是值得研究的
### 4. 三种写回策略（取舍/折中）
* AOF提供三种选项，appendfsync的三个可选值
* always：写回策略由主线程执行，写回时会导致被挂起，此时无法处理新请求，但能最大程度保证内存和磁盘数据一致性。
* everysec：每秒写回，通过后台i/o线程进行，所以主线程不会被阻塞，但内存和硬盘会有一秒的延迟差别，不一定是一秒由线程处理速度决定，所以这是一种折中方案。
* No：将同步内存和硬盘的操作交给操作系统执行，不阻塞主线程，但可能内存和硬盘数据有较大差别。

### 5.性能问题
* 当确定完重写策略时，AOF文件会越来越大，此时会产生AOF文件过大带来的性能问题
* 1. 文件系统本身对文件大小限制，过大无法继续写入
* 2. 文件太大向内部写入效率也会变低
* 3. 发生宕机时，AOF命令需要一个一个被重新执行，效率很慢
### 6.解决AOF文件过大方式：AOF重写机制
* AOF重写机制，会将AOF日志的多个操作融合变成一条最终结果，当恢复数据库时只需要执行这一条语句就可以。
* 虽然重写后文件会变小，但AOF重写过程中会将最新的数据操作日志都写回磁盘，仍是很大的开销。
* AOF重写机制会把当前线程阻塞吗？
* 1. 重写过程由后台子进程bgrewriteaof执行，避免"执行AOF重写的过程"阻塞当前主线程。
* 2. 重写过程可以总结为一个拷贝，两处日志。当执行重写时，主线程将当前内存拷贝（copy on write，实际是先拷贝页表等，主线程写的时候会真的拷贝这个数据，申请一个新内存，这里如果操作的是个bigkey，那么申请大块内存会产生阻塞的风险）一份给子进程，这里面包含了数据库的最新数据，然后子进程进行重新，但主线程还有可能接受新的请求数据，此时这个数据将分别写入两处日志，确保数据都是最新的。当重写成功时，就可以用重新后的AOF日志代替原来日志了。
* 3. 具体过程：首先fork一个子进程，拷贝主进程的页表(内存的页表，内存页是共享的)，然后当有新的数据来的时候，主进程会写原始的aof文件以及如果需要更新原有key的值会用到操作系统的Copy and Write(写时复制技术)，把key对应的数据copy到新的内存(不能影响老的内存值~因为子进程需要写新的aof文件)，当然如果是新key那没有影响，写到新的内存里面，另外主进程还会写一个新的aof缓冲文件(这个文件存储fork子进程之后产生的新数据，就是一行行命令)，同时子进程在写新的aof文件，等到新的aof文件重写完之后，会把新的aof缓冲区数据合并到新的aof文件里(这一步很快)，然后合并完之后就把主线程写的老aof文件切换成新的，这样就完成了一次aof的重写。 
<center>

![](img/04(2).png)

</center>

### 7. AOF重写过程潜在阻塞风险（fork一瞬间拷贝页表进行映射的时候，写时拷贝主线程申请内存时）
1. fork子进程，fork这个瞬间一定是会阻塞主线程的，fork采用操作系统提供的写实复制(Copy On Write)机制，就是为了避免一次性拷贝大量内存数据给子进程造成的长时间阻塞问题，但fork子进程需要拷贝进程必要的数据结构，其中有一项就是拷贝内存页表（虚拟内存和物理内存的映射索引表），这个拷贝过程会消耗大量CPU资源，拷贝完成之前整个进程是会阻塞的，阻塞时间取决于整个实例的内存大小，实例越大，内存页表越大，fork阻塞时间越久。
2. fork出的子进程指向与父进程相同的内存地址空间，此时子进程就可以执行AOF重写，把内存中的所有数据写入到AOF文件中。但是此时父进程依旧是会有流量写入的，如果父进程操作的是一个已经存在的key，那么这个时候父进程就会真正拷贝这个key对应的内存数据，申请新的内存空间，这样逐渐地，父子进程内存数据开始分离，父子进程逐渐拥有各自独立的内存空间。因为内存分配是以页为单位进行分配的，默认4k，如果父进程此时操作的是一个bigkey，重新申请大块内存耗时会变长，可能会产阻塞风险。