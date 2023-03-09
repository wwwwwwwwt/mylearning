<!--
 * @Author: zzzzztw
 * @Date: 2023-03-06 20:51:23
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-07 13:46:16
 * @FilePath: /cpptest/linux网络编程/epoll.md
-->
# epoll的用法
## 使用 epoll 进行网络程序的编写，需要三个步骤，分别是 epoll_create，epoll_ctl 和 epoll_wait
1. epoll_create: 创建并返回一个epoll实例epollfd  
```cpp
int epoll_create(int size);
//size 大于0即可
```
2. epoll_ctl: 在创建完 epoll 实例之后，可以通过调用 epoll_ctl 往这个 epoll 实例增加或删除监控的事件。函数 epll_ctl 有 4 个入口参数。
```cpp
int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);
// 返回值：若成功则返回0，失败返回-1出错
// epfd: 由epoll_create()创建的epoll实例
// op: 表示增加还是删除还是修改监控事件，三个选择：
// 1.EPOLL_CTL_ADD： 向 epoll 实例注册文件描述符对应的事件；  
// 2.EPOLL_CTL_DEL：向 epoll 实例删除文件描述符对应的事件；  
// 3.EPOLL_CTL_MOD： 修改文件描述符对应的事件。
// fd：需要监听的连接套接字

// event结构体使用
epoll_event ev;
ev.data.fd = fd;//连接套接字
ev.events = events;//套接字监听事件和通信模式
//EPOLLIN：表示对应的文件描述字可以读；  
//EPOLLOUT：表示对应的文件描述字可以写；  
//EPOLLRDHUP：表示套接字的一端已经关闭，或者半关闭；  
//EPOLLHUP：表示对应的文件描述字被挂起；  
//EPOLLET：设置为 edge-triggered，边缘触发,EPOLL默认为 level-triggered水平触发。

epoll_ctl(epfd_,EPOLL_CTL_ADD,fd_,&ev);

```
3. epoll_wait: epoll_wait() 函数类似之前的 poll 和 select 函数，调用者进程被挂起，在等待内核 I/O 事件的分发。
```cpp
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout); 
// 返回值: 成功返回的是一个大于0的数，表示事件的个数；返回0表示的是超时时间到；若出错返回-1.

//epfd: epoll_create创造的epoll实例
//events: 自己定义的数组,传入&events[0]
vector<strcut epoll_events> events;
//使用时，根据epoll_wait的返回值，确定个数，遍历events数组， events[i].data.fd得到文件描述符，events[i].events得到文件触发事件

//maxevents:返回的最大时间数量，一般和自己定义的数组长度相等
//timeout: 没有检测到时间最多等待的时间，-1不阻塞，大于零 阻塞等待事件 ms， 和定时器配合使用，阻塞时间是小根堆堆顶的剩余时间
```
## et模式和lt模式
1. epoll_wait 相关的工作流程：  
1.1 当内核监控的fd产生用户关注的事件，内核将fd节点信息加入到就绪队列  
1.2 内核发现就绪队列有数据，开始工作  
1.3 内核先把fd在就绪队列中删除  
1.4 然后将fd对应就绪时间的信息从内核态拷贝到用户态，通过传进去的数组  
1.5 拷贝完成后，内核检查事件模式是lt还是et，lt的话将fd信息重新添加进就绪队列，下次重新触发epoll_wait,无论完成与否，等到下次遍历到这个fd后，发现这里数据处理完了，就continue；
1.6 如果是et模式，数据从内核拷贝到用户空间后，内核不会将其重新加入到就绪队列，这就需要用户在处理时，一次性全部处理完，当用户处理完这个事件后，用户需要根据自己的需求将这个fd通过epoll_ctl_mod重新修改监控事件
1.7 使用et模式，必须在读操作和写操作时一次性读入或写入所有内容，并且设置fd为非阻塞，防止读入写入操作阻塞时，将后续就绪事件全部阻塞，影响效率

## EPOLLONESHOT
1. lt与et模式都可以设置EPOLLONESHOT模式，即使我们使用ET模式，一个fd上的某个事件仍可能被触发多次。比如在多线程程序中，线程A读取完fd上的数据后开始处理这些数据，在处理过程中，fd上又有新数据可读，此时，就算et模式，这个fd会被重新加入就绪队列，导致线程B来读取并处理这些数据；出现了两个线程同时处理一个fd的情况，而我们期望一个fd在任何时候都只被一个线程处理。
2. 我们可以使用epoll_ctl 注册 EPOLLONSHOT 事件，使得同一个fd最多被触发一次事件，这样当一个线程正在处理时，即使其又来了新事件，也不会被处理。当我们处理完已有事件时，必须立即使用epoll_ctl 重新注册 EPOLLONSHOT事件，确保该fd上的事件能重新被触发。