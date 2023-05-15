<!--
 * @Author: zzzzztw
 * @Date: 2023-05-13 15:07:34
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-15 19:15:08
 * @FilePath: /myLearning/etcd/笔记/1ReadandWrite.md
-->
# etcd

### 基础架构
1. client层：提供了API，供客户交互使用
2. API网络层：主要包括http协议和grpc协议，来进行客户与服务器，节点之间通信
3. Raft算法层：实现了leader选举，日志复制等，线性读等功能，用于保证多个etcd节点的数据一致性和服务可用性。
4. 功能逻辑层：包括kvserver模块，mvcc模块， Auth鉴权模块，lease租约模块，Compactor压缩模块。其中mvcc由treeIndex和boltdb共用实现。
5. 存储层: 包括WAL预写日志，快照模块， boltdb模块。其中WAL保证了etcd宕机后数据不丢失，boltdb保存了集群元数据和用户写入数据。


### goreman -f Procfile start

- 快速启动三个节点的本地集群
- 另开一个终端，etcdctl执行命令交互
- “endpoints”是我们后端的 etcd 地址，通常，生产环境下中需要配置多个 endpoints，这样在 etcd 节点出现故障后，client 就可以自动重连到其它正常的节点，从而保证请求的正常执行。
```sh
etcdctl put hello world --endpoints http://127.0.0.1:2379
etcdctl get hello --endpoints http://127.0.0.1:2379
结果：
hello
world
```

- etcd 采用Round-Robin算法轮询，从endpoint列表选择一个endpoint进行长连接访问，进行负载均衡，
- 首先要经过grpc实现的拦截器才能进入到kvserver中，启动的时候讲实现KV的耕种方法都注册到grpc server中，并注册相应的拦截器，拦截器提供了一个请求前后的hook能力，如debug日志，一些接口和参数限制等。都基于拦截器实现。

### 串行读和线性读

#### 串行读：

- client发起一个新更新请求，leader首先接受这个写请求，并将其持久化给WAL日志，并广播给所有节点，若有一半以上节点写到其WAL日志成功，则leader将这个日志条目更新为commit，每个节点的etcdserver模块会异步将其更新数据同步给boltdb状态机。
- 此时，若客户端查询这个请求，轮询到了节点c，他只持久化了日志，但还没更新完其boltdb中，那么客户端从boltdb中读取的就是其旧值。导致了数据的不一致性。

- 总结：串行读适合用在对数据敏感度较低的场景，读请求直接从状态机boltdb中读取数据，效率高。具有低延时、高吞吐量的特点，适合对数据一致性要求不高的场景

#### 线性读

- 为了保证client从这个节点读到的数据是最新的，所以采用ReadIndex机制，首先leader会向所有子节点发送心跳，为了防止脑裂，一般以上节点返回给自己信息才继续执行，然后把自己的最新commit交给读到的这个子节点，子节点则会阻塞，直到自己状态机中的已应用索引追上leader的commit才返回给客户端信息。
- KVServer 模块收到线性读请求后，通过向 Raft 模块发起 ReadIndex 请求，Raft 模块将 Leader 最新的已提交日志索引封装在 ReadState 结构体，通过 channel 层层返回给线性读模块，线性读模块等待本节点状态机追赶上 Leader 进度，追赶完成后，就通知 KVServer 模块。

### MVCC

- treeidex是基于btree实现的，保存用户key与版本号之间的关系
- boltdb是基于b+树的kv数据库，key为版本号，val为详细数据信息。

- 使用treeindex和boltdb做的事务机制保存多个历史版本
- treeindex，是一个B-tree， key为全局递增的版本号，val保存着版本信息
- 拿着这个版本信息去boltdb中找对应的详细信息。从 treeIndex 中获取 key hello 的版本号，再以版本号作为 boltdb 的 key，从 boltdb 中获取其 value 信息。

#### boltdb收到版本号后，先去看下buffer，buffer命中就直接返回，否则就去boltdb查询数据，使用类似bucket的结构对应mysql中的table，将etcd元数据和用户数据隔离出来。

### 写操作流程:

- 首先通过负载均衡选择一个etcd的节点，发起gRpc连接请求，etcd接收到请求后，经过grpc拦截器（封装了一些中间件，如鉴权，压缩等），再经过Quata模块（判断是否写入与内容超过最大etcd db大小） ，然后进入到了kvserver模块，节点向raft模块提交客户发来的key-val，然后通过过广播，给所有节点，如果有一半节点以上持久化成功，就把这个提交设置为commit，表示提交成功，然后传递给Apply模块，通过MVCC更新每个节点的状态机。

#### Quata模块

- 常见报错

```sh
"etcdserver: mvcc: database space exceeded"
```
- 就是由Quata模块发出，表示当前db文件大小超出了配额，整个集群将不能再写入，变为只读状态，影响十分大。
- 解决方法：当QPS很大，k8s集群很大时可能会出现这个状况，解决方法：提升配额：不建议超过8G，检查压缩模块是否开启，这个模块将回收历史版本，将其标记为free下次就可以在他的位置上写入其他数据。


#### KVserver模块

1. 限速：为了避免雪崩，任何提交到raft的请求，都会根据commit idx（已提交索引） - applied idx（已经更新到状态机的索引） > 5000为基础，如果大于了，就说明请求太多了报错。
2. 生成一个全局ID，并关联到一个新的消息请求，内容为写入的key-val，然后向raft提交这个，让其广播。如果超时会报错
3. WAL：raft模块接收到请求后，如果当前节点是子节点，需要将这个请求转发给leader，只有leader才能处理写请求，leader收到后将其持久化，并广播给所有子节点，超过半数持久化成功后，将其更新为commit。

#### Apply模块

1. 用于kvserver与mvcc中间的缓冲地带，鉴定这个请求被mvcc持久化过了吗，保证幂等。

#### MVCC模块：

- 包括Treeidx 和bolt两部分组成，treeidx是一个内存b树，每个节点保存着对应的key和版本号。
- boltdb是一个b+树，key为版本号，val为值，租约信息等。
- 只更新在了boltdb内存中，并没有实际落到磁盘。每隔100ms会自动刷盘。