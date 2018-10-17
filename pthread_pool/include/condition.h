#ifndef __CONDITION_H_
#define __CONDITION_H_
#include <pthread.h>
#include <sys/time.h>

//进程池环境变量
typedef struct condition{
	pthread_mutex_t thread_mutex;
	pthread_cond_t thread_cond;
}condition_t;

int condition_init(condition_t *condition);//初始化环境变量
int condition_lock(condition_t *condition);//加锁
int condition_unlock(condition_t *condition);//解锁
int condition_wait(condition_t *condition);//等待一个环境变量
int condition_signal(condition_t *condition);//发送信号给一个空闲线程
int condition_broadcast(condition_t *condition);//发送信号给所有的空闲线程
int condition_destroy(condition_t *condition);//释放互斥量与条件变量
int condition_timewait(condition_t *condition);//在一定时间内等待环境变量
#endif
