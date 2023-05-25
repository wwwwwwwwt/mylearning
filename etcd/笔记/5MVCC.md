<!--
 * @Author: zzzzztw
 * @Date: 2023-05-24 14:23:29
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-24 19:05:10
 * @FilePath: /myLearning/etcd/笔记/5MVCC.md
-->
# MVCC

#### 多版本并发控制
* 整个MVCC特性，由基于B-tree的treeIdex 和 基于B+tree的boltdb 和对应的APPLY模块组成
* etcd每次在修改key时会自动生成一个全局递增的版本号，然后treeIdex会保存key与版本号之间的关系。再用版本号作为boltdb中的key，用户key-val作为value。
* 删除模式为懒删除，第一次删除只是标记并清空这个key的各个版本，真正删除是由压缩组件异步删除的。
* 我理解etcd采用延迟删除，1是为了保证key对应的watcher能够获取到key的所有状态信息，留给watcher时间做相应的处理。2是实时从boltdb删除key，会可能触发树的不平衡，影响其他读写请求的性能。