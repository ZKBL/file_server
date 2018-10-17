#include "condition.h"
#include <stdio.h>

int condition_init(condition_t *condition){
	int ret ;
	if((ret =pthread_mutex_init(&condition->thread_mutex,NULL))){
		perror("mutex_init");
	}
	if((ret=pthread_cond_init(&condition->thread_cond,NULL))){
		perror("condition_init");
	}
	return 0;
}
int condition_destory(condition_t *condition){
	if(pthread_mutex_destroy(&condition->thread_mutex)){
		perror("mutex destroy");
		return -1;
	}
	if(pthread_cond_destroy(&condition->thread_cond)){
		perror("mutex destroy");
		return -1;
	}
	return 0;
}
int condition_lock(condition_t *condition){
	return pthread_mutex_lock(&condition->thread_mutex);
}
int condition_unlock(condition_t *condition){
	return pthread_mutex_unlock(&condition->thread_mutex);
}
int condition_wait(condition_t *condition){
	return pthread_cond_wait(&condition->thread_cond,&condition->thread_mutex);
}
int condition_signal(condition_t *condition){
	return pthread_cond_signal(&condition->thread_cond);
}
int condition_broadcast(condition_t *condition){
	return pthread_cond_broadcast(&condition->thread_cond);
}

int condition_timewait(condition_t *condition,const struct timespec *ts){
		return pthread_cond_timedwait(&condition->thread_cond,&condition->thread_mutex,ts);
}
