<!--
 * @Author: zzzzztw
 * @Date: 2023-05-22 10:58:22
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-23 16:08:26
 * @FilePath: /myLearning/etcd/笔记/4租约与clientv3.md
-->
# 租约

### 什么是Lease 
* etcd的lease属于主动上报的模式，提供一种活性检测机制，client与etcd server之间存在一个约定，内容是etcd server保证在约定的个有效期内（TTL）,不会删除你关联到此lease上的key-value。
* 若在到期还没续租，etcd server就会删除lease和关联的key-value

** 可以基于leaseTTL的特性，解决类似leader选举，k8s event淘汰，注册中心服务风险等业务。

#### Lease整体架构

* etcd 启动时会开两个协程出去一个用于检测过期的租约，一个定时触发更新租约剩余到期时间, 原理时小根堆，每次取出第一个看过期时间
* 四个大API
  * Grant：创建一个TTL为指定秒数的租约，这个租约的信息会被持久化到boltdb中
  * Revoke：撤销一个租约并删除其关联数据
  * Lease Timetolive：获取一个租约剩余时间
  * Lease KeepAlive: 给一个租约续期
```sh
# 创建一个TTL为600秒的lease，etcd server返回LeaseID
$ etcdctl lease grant 600
lease 326975935f48f814 granted with TTL(600s)


# 查看lease的TTL、剩余时间
$ etcdctl lease timetolive 326975935f48f814
lease 326975935f48f814 granted with TTL(600s)， remaining(590s)
```

### Go实现租约需要基于clientv3提供的API

### 1. 初始化client

```go
cli, err := clientv3.New(clientv3.Config{
    EndPoint: []string{"localhost:2379"}, // etcd的多个节点的服务地址
    DialTimeout : 5 * time.Second， //创建client的首次连接超时，如果超时还没成功就会返回err。
})

```
client结构体包括如下几个部分，cluster，KV, Lease， Wathcer， Auth， Maintenance。username 和 Password

* Cluster : 向集群里增加etcd服务端节点之类，属于管理员操作。
* KV: 主要是要的功能， k-v操作
* Lease：租约相关操作，比如申请一个TTL = 10s的租约
* Wacher： 观察订阅，从而监听最新数据变化
* Auth，： 管理etcd的用户和权限
* MaintenanceL:迁移节点和数据等

```go
type Client struct {
    Cluster
    KV
    Lease
    Watcher
    Auth
    Maintenance

    // Username is a user name for authentication.
    Username string
    // Password is a password for authentication.
    Password string
    // contains filtered or unexported fields
}

```

### 2. 通过clientv3获取KV对象
* clientv3。KV提供的API
```go
type KV interface {
	Put(ctx context.Context, key, val string, opts ...OpOption) (*PutResponse, error)

	Get(ctx context.Context, key string, opts ...OpOption) (*GetResponse, error)

	Delete(ctx context.Context, key string, opts ...OpOption) (*DeleteResponse, error)

	Compact(ctx context.Context, rev int64, opts ...CompactOption) (*CompactResponse, error)
	Do(ctx context.Context, op Op) (OpResponse, error)

	Txn(ctx context.Context) Txn
}
```

* 获得一个被包装的kv,自带错误重试机制

```go
kv := clientv3.NewKV(cli) // 之前创造的client
```

* PUT操作
```go
func Put(ctx context.Context, key, val string, opts ...OpOption) (*PutResponse, error)
// 第一个参数context用于设置超时返回，后面2个参数分别是key和value，
// 还支持一个可变参数，主要是传递一些控制项来影响Put的行为，例如可以携带一个lease ID来支持key过期，

//返回值 PutResponse,不同的KV操作对应不同的response结构
type (
	CompactResponse pb.CompactionResponse
	PutResponse     pb.PutResponse
	GetResponse     pb.RangeResponse
	DeleteResponse  pb.DeleteRangeResponse
	TxnResponse     pb.TxnResponse
)


//example
if putResp, err = kv.Put(ctx, "/illusory/cloud", "hello"); err != nil {
		fmt.Println(err)
		return
	}
```
* GET操作

```go
func Get(ctx context.Context, key string, opts ...OpOption) (*GetResponse, error)
//和Put类似，函数注释里提示我们可以传递一些控制参数来影响Get的行为
//比如：WithFromKey表示读取从参数key开始递增的所有key，而不是读取单个key。

//example
// 用kv获取Key 获取前缀为/illusory/的 即 /illusory/的所有孩子
//withPrefix实际上会转化为范围查询，它根据前缀生成了一个key range，
	if getResp, err = kv.Get(con, "/illusory/",clientv3.WithPrefix()); err != nil {
		fmt.Println(err)
	} 
```


### 3. 获取lease对象

* clientv3，提供的lease的API
  * Grant: 分配一个新租约
  * Revoke：删除一个租约
  * TimeToLive：获取剩余TTL时间
  * Leases

```go
type Lease interface {
	Grant(ctx context.Context, ttl int64) (*LeaseGrantResponse, error)

	Revoke(ctx context.Context, id LeaseID) (*LeaseRevokeResponse, error)

	TimeToLive(ctx context.Context, id LeaseID, opts ...LeaseOption) (*LeaseTimeToLiveResponse, error)

	Leases(ctx context.Context) (*LeaseLeasesResponse, error)

	KeepAlive(ctx context.Context, id LeaseID) (<-chan *LeaseKeepAliveResponse, error)

	KeepAliveOnce(ctx context.Context, id LeaseID) (*LeaseKeepAliveResponse, error)

	Close() error
}
```

* 获取一个lease对象模块

```go
//etcd 在启动 lease 的时候会启动一个 goroutine revokeExpiredLeases(),他会没500毫秒执行一次清除操作。
lease := clientv3.NewLease(cli)
```

* 创建一个拥有10秒TTL的租约

```go
grantrsp, err := lease.Grant(context.TODO(),10)

// geantrsp中包括的信息
type LeaseGrantResponse struct {
	*pb.ResponseHeader
	ID    LeaseID
	TTL   int64
	Error string
}
主要使用到了ID，也就是租约ID
```

* 利用租约，像kv中put一个10s会自动过期的key

```go
	// 用client也可以设置key，kv是client的一个结构，因此可以使用其方法
	if putResp, err = kv.Put(context.TODO(), "/illusory/cloud/x", "ok", clientv3.WithLease(leaseResp.ID)); err != nil {
		fmt.Println(err)
	}
```

* 利用租约续租功能实现服务注册
  * 当我们实现服务注册时，需要主动给Lease进行续约，这需要调用KeepAlive/KeepAliveOnce
  * KeepAlive:自动定时的续约某个租约。
  * KeepAliveOnce:为某个租约续约一次
```go
	// 主动给Lease进行续约
  // 有协程来帮自动续租，每秒一次，协程内部再开启两个协程，一个发起续租，一个检删等待太久没反馈的租约
	if keepAliveChan, err := client.KeepAlive(context.TODO(), leaseResp.ID); err != nil { 
		fmt.Println(err)
	}

  //需要提供一个消耗心跳的方法，防止chan阻塞
  func leaseKeepAlive(response <-chan *clientv3.LeaseKeepAliveResponse) {
	// 不断地取出续租成功的消息，避免塞满，一般是 1次/秒
	for true {
		select {
		case ret := <-response:
			if ret == nil {
				return
			}
		}
	}
}

```

