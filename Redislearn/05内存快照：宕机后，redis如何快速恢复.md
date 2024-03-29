<!--
 * @Author: zzzzztw
 * @Date: 2023-03-21 14:50:06
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-21 16:05:33
 * @FilePath: /cpptest/Redislearn/05内存快照：宕机后，redis如何快速恢复.md
-->
# 内存快照
* 所谓内存快照就是指内存中的数据在某一时刻的状态记录，类似照片，当你给朋友拍照时，一张照片就能把朋友一瞬间的形象完全记下来
* 对于redis来说，实现类似照片的效果就是把某一时刻的状态以文件形式记录到磁盘上，也就是快照。即使宕机快照文件也不会丢失，数据的可靠性也得到了保证，这个文件叫做RDB文件，redis Database的缩写。

## 两个关键问题
* 对哪些数据做快照，关系到快照的执行效率
* 做快照时，数据还能被增删改吗，这关系到redis是否被阻塞，能否正常被处理请求  

以上两个问题可以拿照片举例子，比如我们在拍照时，通常关注两个问题：
* 如何取景，打算把哪些人拍进照片
* 按快门前，要提醒朋友不要乱动，否则就模糊了  

对于redis而言，单线程模型决定了，我们要尽量避免所有会阻塞主线程的操作关系到降低redis的性能
* redis提供了两个命令来生成RDB文件：save 和 bgsave，bgsave后台调用子线程进行快照操作

## 我们要关注的是，当进行快照时，这些数据还能动吗？
* redis使用写时拷贝的技术，在执行快照的同时，还能继续正常处理写操作
* bgsave 由主线程fork产生，进行读操作时，主线程和子进程都对一份数据进行操作，当主线程是写操作时，主线程将把这份数据复制一份，并指向新复制的这份数据，进行写操作；bgsave仍对原先的数据进行快照，写入RDB文件。

## 可以每秒都进行快照吗，这样丢失的数据很少
* 不可以，频繁执行全局快照有两方面开销
* 1. 写磁盘的开销，前一个没写完后一个接着写了
* 2. 主线程fork时的开销，这个操作会阻塞主线程。并且操作数据时，如果操作的数据很大也会有相应的申请资源拷贝数据的消耗，产生潜在阻塞。
  
## 将AOF日志和快照结合，效率最好
* 设置的参数是： aof-use-rdb-preamble yes
* 简单来说，内存快照以一定频率执行，两次快照之间使用AOF日志记录所有操作，下一次快照结束，这个日志就可以删了，也避免了AOF重写开销（见上一篇）。

# 做RDB持久化，产生的风险在于内存资源量风险和cpu两方面
1.  内存资源风险：Redis fork子进程做RDB持久化，由于写的比例为80%，那么在持久化过程中，“写实复制”会重新分配整个实例80%的内存副本，大约需要重新分配1.6GB内存空间，这样整个系统的内存使用接近饱和，如果此时父进程又有大量新key写入，很快机器内存就会被吃光，如果机器开启了Swap机制，那么Redis会有一部分数据被换到磁盘上，当Redis访问这部分在磁盘上的数据时，性能会急剧下降，已经达不到高性能的标准（可以理解为武功被废）。  
2.  CPU资源风险：虽然子进程在做RDB持久化，但生成RDB快照过程会消耗大量的CPU资源，虽然Redis处理处理请求是单线程的，但Redis Server还有其他线程在后台工作，例如AOF每秒刷盘、异步关闭文件描述符这些操作。由于机器只有2核CPU，这也就意味着父进程占用了超过一半的CPU资源，此时子进程做RDB持久化，可能会产生CPU竞争，导致的结果就是父进程处理请求延迟增大，子进程生成RDB快照的时间也会变长，整个Redis Server性能下降。