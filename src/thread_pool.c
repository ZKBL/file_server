#include "thread_pool.h"
#include "condition.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

void *threadpool_thread(void	*pool);


thread_pool_t *threadpool_init(int thread_max_size,int queue_max_size){

	thread_pool_t 				*pool;
	pool=(thread_pool_t *)malloc(sizeof(thread_pool_t));
	pool->thread_max_size=thread_max_size;
	pool->thread_work_size=0;
	pool->thread_idle_size=0;
	pool->task_max_size=queue_max_size;
	pool->tail=NULL;
	pool->head=NULL;
	pool->shutdown=POOL_OPEN;

	return pool;
}
/*
   int threadpool_create(thread_pool_t *pool){
   condition_init(&pool->ready);
   pool->thread_id=(pthread_t*)malloc(sizeof(pthread_t)*pool->thread_max_size);
   for (int i=0;i<pool->thread_max_size;i++){
   if((pthread_create(&(pool->thread_id[i]),NULL,threadpool_thread,(void *)pool))!=0){
   perror("pthread_create");
//threadpool_destory(pool,0);
return -1;
}
}
return 0;
}
*/
int threadpool_add_task(thread_pool_t *pool,TASK_ROUTINE my_func,TASK_ARG arg){

	//创建新任务
	task_t *new_task;
	new_task=(task_t*)malloc(sizeof(task_t));
	new_task->run=my_func;
	new_task->next=NULL;
	new_task->arg=arg;


	//加锁
	condition_lock(&pool->ready);

	//添加到任务队列
	if(pool->head){
		pool->tail->next=new_task;
	}
	else{
		pool->head=new_task;
	}
	pool->tail=new_task;

	//通知等待的线程
	if(pool->thread_idle_size>0){
		condition_signal(&pool->ready);
	}
	else if(pool->thread_work_size<pool->thread_max_size){
		pthread_t tid;
		if(pthread_create(&tid,NULL,threadpool_thread,(void*)pool)){
			perror("pthread create");
			return -1;
		}
		printf("debugg\n");
		pool->thread_work_size++;
	}

	//解锁
	condition_unlock(&pool->ready);
	return 0;
}

int threadpool_destroy(thread_pool_t *pool){

	//如果进程池关闭直接返回
	if(pool->shutdown==POOL_CLOSE){
		return 0;
	}
	//加锁
	condition_lock(&pool->ready);
	//进程池状态改为关闭
	pool->shutdown=POOL_CLOSE;

	//等待工作任务完成
	if(pool->thread_work_size>0){
		if(pool->thread_idle_size)
			condition_broadcast(&pool->ready);
		while(pool->thread_work_size){
			condition_wait(&pool->ready);
		}
	}
	//解锁
	condition_unlock(&pool->ready);
	//清除锁和条件变量
	condition_destroy(&pool->ready);
	return 0;
}

void* threadpool_thread(void *arg){

	int 					ret;
	pthread_t 				pid;
	int 					timeout;

	pid=pthread_self();
	printf("Thread %#lx starting\n",(size_t)pid);
	thread_pool_t *pool=(thread_pool_t*)arg;

	for(;;){
		timeout=0;
		condition_lock(&pool->ready);
		pool->thread_idle_size++;

		//等待新任务或者摧毁线程池
		while((pool->head==NULL)&&(pool->shutdown==POOL_OPEN)){
			printf("Thread %#lx is waiting.\n",pid);
			struct timeval 			time_now_get;
			struct timespec 				time_now;
			gettimeofday(&time_now_get,NULL);
			time_now.tv_sec=time_now_get.tv_sec+5;
			time_now.tv_nsec=0;

			ret=condition_timewait(&pool->ready,&time_now);
			if(ret==ETIMEDOUT){
				printf("Thread %#lx wait timeout\n",(size_t)pid);
				timeout=1;
				break;
			}
		}

		pool->thread_idle_size--;
		//处理队列头的任务
		if(pool->head){
			task_t *task=pool->head;
			pool->head=task->next;
			condition_unlock(&pool->ready);
			task->run(task->arg);
			free(task);
			task=NULL;
			condition_lock(&pool->ready);
		}

		//destory thread pool
		if((pool->shutdown==POOL_CLOSE)&&(pool->head==NULL)){
			pool->thread_work_size--;
			if(pool->thread_work_size==0){
				condition_signal(&pool->ready);
			}
			condition_unlock(&pool->ready);
			break;
		}
	}
	printf("Thread %#lx exiting\n",(size_t)pid);
	return 0;
}
