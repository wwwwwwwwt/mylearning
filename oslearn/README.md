<!--
 * @Author: zzzzztw
 * @Date: 2023-02-28 15:13:17
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-02-28 16:23:20
 * @FilePath: /cpptest/操作系统/README.md
-->
# 进程、线程和协程
## 1. 进程、线程和协程的区别和联系
1. 进程：进程是分配资源的最小单位，运行一个可执行程序会创建一个或多个进程，进程就是运行起来的可执行程序，切换状态：用户态->内核态->用户态。
当发生切换时，进程会先保存当前cpu的环境（栈，寄存器，页表和文件句柄等）再调度新的进程cpu环境的设置。不同进程之间切换实现并发，各自占有cpu实现并行。
切换消耗大，进程间通信需要利用操作系统比如：管道，共享内存等。
2. 线程：线程时轻量级的进程，是程序执行的基本单位。由内核进行切换，切换状态：用户态->内核态->用户态。切换时线程保存和设置程序计数器、少量寄存器和栈的内容。线程间可以采用读写进程数据段（如全局变量）进行通信。一个进程间可以有多个线程进行并发。
3. 协程：用户态的轻量级协程，可以理解为一个可以保存状态的函数，而线程是执行函数的。由用户自己决定切换时机，切换时只在用户态间进行切换没有陷入内核态。同一时间只能执行一个协程，其他协程处于休眠，由调度器决定协程的切换。效率最快。
## 2. 线程和进程之间的比较和区别
1. 线程轻量级启动速度快，系统开销小，同一进程中不同线程共享的有全局变量、静态变量、引用等，但独享栈空间。
2. 调度：线程是调度的基本单位（pc码，状态码，通用寄存器，线程栈，栈指针）；进程是拥有资源的基本单位（堆，静态区，代码段等）
3. 拥有资源：线程不拥有系统资源，但一个进程的多个线程可以共用隶属进程的全局变量；进程是拥有资源的最小单位
4. 系统开销：线程创建销毁只需要处理pc值，状态码，通用寄存器，线程栈和栈指针；进程创建和销毁会需要重新分配和销毁task_struct结构.
## 3.进程的组成
1. 包括：栈，堆，代码段（静态变量存放的地方），pc(program counter)指向下一个要运行的指令，寄存器（进程切换时保存运行状态，下次继续运行）。
## 4. 一个进程可以创建多少个线程
1. 32位系统用户态虚拟内存3G，创建线程默认10M，约300个线程。
2. 64位系统用户态虚拟内存128T，理论上不会受内存大小限制，而是系统参数和性能定义。
3. 并不是线程越多越好，过多的线程会把时间浪费在线程切换上，因为用户态到内核态切换很慢，一般多线程并发时采用线程池。
## 5.外中断和异常有什么区别
1. 外中断是指由CPU执行指令以外的事件引起，如i/o完成中断，表示设备输入输出已经完成，处理器能开始下一个i/o请求。还有时钟中断，控制台中断等。
2. 异常是由cpu执行指令的内部事件引起的，如数组越界，除零，非法操作等。