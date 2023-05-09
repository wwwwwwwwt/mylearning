<!--
 * @Author: zzzzztw
 * @Date: 2023-05-09 19:09:32
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-09 22:26:26
 * @FilePath: /myLearning/mysql45/22mysql如何保证数据不丢.md
-->
# mysql如何保证数据不丢

### 只要redo log 和binlog 持久化到磁盘，就能保证mysql异常重启后数据可以得到恢复。

### 1. binlog的 写入机制

- 每个线程的的日志都有一个binlog cache共用一个binlog，写入的时候先写入binlog cache，速度很快，事务提交时，再从binlog cache写入fsync到binlog中，相当于数据持久化到磁盘并清空binlog cache。
- write写入binlog cache的时机和fsync持久化到磁盘的时机有sync_binlog参数设定：
  - sync_binlog = 0, 每次提交事务只write，不fsync
  - sync_binlog = 1, 每次提交都执行fsync
  - sync_binlog = N, 表示每次提交事务都write，累计N个事务进行fsync
- 一般设为100 - 1000，但发生重启异常会丢失N个事务的binlog日志

### 2. redo log的写入机制

- 每个线程共享一个redo log buffer
- redo log 也有三种写入策略，innodb_flush_log_at_trx_commit
  - innodb_flush_log_at_trx_commit = 0，每次提交都把redo log留在redo log buffer中
  - innodb_flush_log_at_trx_commit = 1, 每次事务提交都把redo log 写到磁盘中
  - innodb_flush_log_at_trx_commit = 2, 每次事务提交都把redo log写道page_cache系统缓存，mysql异常不会收到影响
- 此外，innodb后台还有一个线程，每隔一秒就把redo log buffer中的日志调用write写入page cache然后调用fsync持久化到磁盘。！！！注意，事务执行期间，redo log直接写在redo log buffer中的，也会被后台线程持久化到磁盘中，但由于没有commit标识，所以可以认为这个是垃圾语句，回档时会忽略。
- 除了后台线程每秒刷盘，第二种情景是redo log buffer占用了一半以上内存时，会主动刷盘清空，第三种情况是并行事务提交时，也会将这个redo log buffer持久化磁盘。

** 补充两阶段提交知识：
1. redolog在prepare阶段持久化到磁盘（可能失败） 
2. 紧接着binlog持久化（可能失败） 3
3. 最后redolog commit（可能失败） 
4. 情况一：在1处mysql异常重启，redo log没有fsync，内存丢失，直接回滚，不影响数据一致性； 
5. 情况二：redolog fsync成功，但是binlog写入错误，此时mysql异常重启，现在有redolog的磁盘数据没有binlog的数据，此时检测redolog处于prepare阶段，但是没有binlog，回滚（虽然刚刚redolog fsync了但是不影响数据一致性，因为redolog的操作并没有写入mysql，也永远不会写入mysql）； 
6. 情况三：binlog完整但未commit，此时检测redolog处于prepare阶段，且binlog完整但未提交，默认添加commit标记，进而提交，写入mysql，满足数据一致性； 情况四：binlog完整且提交，写入musql，满足一致性；


** 按组提交提高(group commit)提高写磁盘能力，实际上两阶段提交分为四步，1. redo log prepare /write || 2. binlog write || 3. redo log prepare fsync || 4. binlog fsync || 5. relog log commit write
