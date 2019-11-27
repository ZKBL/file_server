//
//  thread_pool.h
//  file_server
//
//  Created by 冯蕗铭 on 2019/9/11.
//  Copyright © 2019 冯蕗铭. All rights reserved.
//

#ifndef thread_pool_h
#define thread_pool_h
#include <pthread.h>
#include <stdbool.h>

typedef struct task{
    void * (*work_func)(void *arg);
    void * args;
    struct task *next;
}Task;

typedef struct threadpool{
    size_t          max_pthread_num;
    bool            pool_is_shutdown;
    pthread_t       *thread_id;
    Task            *task_queue_head;
    pthread_cond_t  queue_ready;
    pthread_mutex_t queue_lock;
}ThreadPool;

int create_pool(ThreadPool ** pool,size_t max_thread_num);
void destory_pool(ThreadPool* pool);
int add_task_to_threadpool(ThreadPool *pool,void* (*task_func)(void *),void *args);
#endif /* thread_pool_h */
