/*************************************************
* 进程池管理器：用于创建和管理进程池
* 包括创建进程池、销毁进程池、添加新任务
***************************************************/

#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include <pthread.h>

#include "condition.h"

#define POOL_CLOSE 0
#define POOL_OPEN 1
//任务队列节点
typedef void* (*TASK_ROUTINE)(void*);
typedef void* TASK_ARG;
typedef struct task{
	TASK_ROUTINE run;
	TASK_ARG arg;
	struct task *next;
}task_t;
/*
线程池的结构定义
ready  							condition结构体，用于控制任务的执行
thread_max_size					最大线程数目
thread_work_size				工作线程数目
thread_idle_size				空闲线程数目
task_max_size					最大工作队列数目
head							task_t结构体指针，任务队列头指针
tail							task_t结构体指针，任务队列尾指针
shutdown						进程池是否关闭

*/
//线程池结构体
typedef struct threadpool{
	condition_t 			ready;							
	int 					thread_max_size;
	int 					thread_work_size;
	int 					thread_idle_size;
	int 					task_max_size;
	task_t 					*head;
	task_t 					*tail;
	pthread_t				*thread_id;
	int 					shutdown;
}thread_pool_t;

//初始化进程池
thread_pool_t *threadpool_init(int thread_max_size,int queue_max_size);

//int threadpool_create(thread_pool_t *pool);

//销毁进程池
int threadpool_destroy(thread_pool_t *pool);

//向进程池中添加任务
int threadpool_add_task(thread_pool_t *pool,TASK_ROUTINE my_func,TASK_ARG arg);



#endif

