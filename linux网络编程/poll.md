<!--
 * @Author: zzzzztw
 * @Date: 2023-03-07 14:28:05
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-07 14:33:17
 * @FilePath: /cpptest/linux网络编程/poll.md
-->
# poll的用法
1. poll 函数介绍
poll 是除了 select 之外，另一种普遍使用的 I/O 多路复用技术，和 select 相比，它和内核交互的数据结构有所变化，另外，也突破了文件描述符的个数限制。
```c
int poll(struct pollfd *fds, unsigned long nfds, int timeout); 
　　　
返回值：若有就绪描述符则为其数目，若超时则为0，若出错则为-1
```
这个函数里面输入了三个参数，第一个参数是一个 pollfd 的数组。其中 pollfd 的结构如下：
```cpp
struct pollfd {
    int    fd;       /* file descriptor */
    short  events;   /* events to look for */
    short  revents;  /* events returned */
 };
```
这个结构体由三个部分组成，首先是描述符 fd，然后是描述符上待检测的事件类型 events，注意这里的 events 可以表示多个不同的事件，具体的实现可以通过使用二进制掩码位操作来完成，例如，POLLIN 和 POLLOUT 可以表示读和写事件  

2. 和 select 非常不同的地方在于，poll 每次检测之后的结果不会修改原来的传入值，而是将结果保留在 revents 字段中，这样就不需要每次检测完都得重置待检测的描述字和感兴趣的事件。我们可以把 revents 理解成“returned events”  
events 类型的事件可以分为两大类。第一类是可读事件，有以下几种：
```cpp
#define POLLIN     0x0001    /* any readable data available */
#define POLLPRI    0x0002    /* OOB/Urgent readable data */
#define POLLRDNORM 0x0040    /* non-OOB/URG data available */
#define POLLRDBAND 0x0080    /* OOB/Urgent readable data */
```
一般我们在程序里面有 POLLIN 即可。套接字可读事件和 select 的 readset 基本一致，是系统内核通知应用程序有数据可以读，通过 read 函数执行操作不会被阻塞  

第二类是可写事件，有以下几种：
```cpp
#define POLLOUT    0x0004    /* file descriptor is writeable */
#define POLLWRNORM POLLOUT   /* no write type differentiation */
#define POLLWRBAND 0x0100    /* OOB/Urgent data can be written */
```
一般我们在程序里面统一使用 POLLOUT。套接字可写事件和 select 的 writeset 基本一致，是系统内核通知套接字缓冲区已准备好，通过 write 函数执行写操作不会被阻塞。  

3. 我们再回过头看一下 poll 函数的原型。  
1.参数 nfds 描述的是数组 fds 的大小，简单说，就是向 poll 申请的事件检测的个数。  
最后一个参数 timeout，描述了 poll 的行为。如果是一个 <0 的数，表示在有事件发生之前永远等待；如果是 0，表示不阻塞进程，立即返回；如果是一个 >0 的数，表示 poll 调用方等待指定的毫秒数后返回。  
2.关于返回值，当有错误发生时，poll 函数的返回值为 -1；如果在指定的时间到达之前没有任何事件发生，则返回 0，否则就返回检测到的事件个数，也就是“returned events”中非 0 的描述符个数。  
3.poll 函数有一点非常好，如果我们不想对某个 pollfd 结构进行事件检测，可以把它对应的 pollfd 结构的 fd 成员设置成一个负值。这样，poll 函数将忽略这样的 events 事件，检测完成以后，所对应的“returned events”的成员值也将设置为 0。  

4. 与select不同
和 select 函数对比一下，我们发现 poll 函数和 select 不一样的地方就是，在 select 里面，文件描述符的个数已经随着 fd_set 的实现而固定，没有办法对此进行配置；而在 poll 函数里，我们可以控制 pollfd 结构的数组大小，这意味着我们可以突破原来 select 函数最大描述符的限制，在这种情况下，应用程序调用者需要分配 pollfd 数组并通知 poll 函数该数组的大小。