<!--
 * @Author: zzzzztw
 * @Date: 2023-07-07 15:50:09
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-07-07 16:47:43
 * @FilePath: /myLearning/freeRTOS/learning.md
-->
# freeRTOS学习笔记

1. 任务概念  

任务是没有返回值的，参数一般是一个指针变量，任务主体部分·是无限循环且不能返回，类似如下

```c
void task_entry(void* parg){
    
    for(;;){
        //主任务代码
    }
}

```
   
定义任务栈， 是Ram中一段连续的内存空间，必须为每个任务都定义任务栈空间，是一个定义的全局数组，单位是128字 = 512字节

```c
#define TASK_STACK_SIZE 128

STACKType_t Task1Stack[TASK_STACK_SIZE];

```

定义任务函数

```c
void task1_task(void *pvRarag){
    while(1){
        LED1=0;
        vTaskDelay(200);
        LED1=1;
        vTaskDelay(200);
    }
}
```

任务控制块：相当于任务的身份

```c
typedef struct tskTaskControlBlock
{
    volatile StackType_t *pxTopOfSatck; // 栈顶指针
    ListItem_t xStateListItem; /*任务节点*/
    StackType_t *pxSatck;//任务栈起始地址
    /*任务名称，字符串形式*/
    char pcTaskName[config_task_name_len];
}

typedef tskTCB TCB_t;

```

列表和列表项，列表相当于链表，列表项相当于c中的中的双向链表，列表包括列表项的idx起始位置，用于查找列表项，还有最后一个列表项，类似尾节点。总结一下列表是一个带有成员idx，环形，最后一个的下一个就是开头和尾地址的链表（类似vector）将列表项（双向链表：按值大小单调递增的链表，归谁拥有，前后指针）穿起来的

* 列表的初始化
  ```c
  传入我们想要初始化的列表
  void vListInitialise(List* const pxList){

  }

  ```

* 列表项的初始化
    ```C
    void ListInitialiseItem(ListItem* const pxItem){

    }

    ```
* 列表项的插入删除和遍历

    ```c
    第一个参数是想要插入的列表，第二个是插入的列表项，按照插入列表项的值大小，插入到对应的合适位置
    void vListInsert(List* const pxList, ListItem_t* const pxNewListItem){
        
    }
    //将列表项插入到列表末尾
    void vListInsertEnd(List* const pxList, ListItem_t* const pxNewListItem){
        
    }
    //列表项删除,参数是准备删除的列表，返回列表删除这个列表项后剩余的列表项数目
    UBaseType_T ListRemove(ListItem* const pxItemTPremove){

    }
    //每调用一次idx就指向下一个列表项，并返回这个列表项剩余个数
    #define listCET_OWNER_OF_NEXT_ENTRY(pxTCB, pxList)
    ```
任务创建：  
TaskFunctin_t pxTaskCode：任务函数指针  
const char* const pxName：任务名称  
const uint32_t ulStackSize,：任务堆栈大小,单位是字
void *const pvParameters：任务函数的参数 
UbaseType_t uXPriority：任务优先级
StackType_t *const puxStackBuffer 任务栈的起始地址
StackTaskType_t *const pxTaskBuffer 任务控制块的指针
```c
void xTaskCreateStatic(TaskFunctin_t pxTaskCode, 
const char* const pxName, 
const uint32_t ulStackSize,
void *const pvParameters,
UbaseType_t uXPriority, 
StackType_t *const puxStackBuffer,
StackTaskType_t *const pxTaskBuffer){   
}
```  
