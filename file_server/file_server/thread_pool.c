//
//  thread_pool.c
//  file_server
//
//  Created by 冯蕗铭 on 2019/9/11.
//  Copyright © 2019 冯蕗铭. All rights reserved.
//

#include "thread_pool.h"
#include <stdlib.h>
#include <stdio.h>
static void *(thread_func)(void *args){
    ThreadPool *pool = (ThreadPool *)args;
    Task * task= NULL;
    while(1){
        pthread_mutex_lock(&pool->queue_lock);
        while(pool->pool_is_shutdown == false && pool->task_queue_head == NULL)
        {
            pthread_cond_wait(&pool->queue_ready, &pool->queue_lock);
        }
        if(pool->pool_is_shutdown==true){
            pthread_mutex_unlock(&pool->queue_lock);
            pthread_exit(NULL);
        }
        
        task = pool->task_queue_head;
        pool->task_queue_head=pool->task_queue_head->next;
        pthread_mutex_unlock(&pool->queue_lock);
        
        task->work_func(task->args);
        free(task);
    }
    return NULL;
}

int create_pool(ThreadPool **pool,size_t max_thread_num){
    (*pool)=(ThreadPool *)malloc(sizeof(ThreadPool));
    if(NULL == *pool){
        perror("malloc ThreadPool filed!");
        exit(-1);
    }
    (*pool)->pool_is_shutdown=false;
    (*pool)->max_pthread_num=max_thread_num;
    (*pool)->thread_id=(pthread_t*)malloc(sizeof(pthread_t)*max_thread_num);
    if((*pool)->thread_id == NULL){
        perror("init thread_id failed !");
        exit(-1);
    }
    (*pool)->task_queue_head=NULL;
    if(pthread_mutex_init(&((*pool)->queue_lock), NULL) !=0 ){
        perror("initial mutex failed!");
        exit(-1);
    }
    if(pthread_cond_init(&((*pool)->queue_ready), NULL) !=0 ){
        perror("initial condition variable failed");
        exit(-1);
    }
    for(int index=0 ;index <max_thread_num;++index){
        if(pthread_create(&((*pool)->thread_id[index]), NULL, thread_func, (void*)(*pool))){
            perror("pthread_create failed!");
            exit(-1);
        }
    }
    return 0;
}

void destory_pool(ThreadPool *pool){
    Task *tmp_task;
    if(pool->pool_is_shutdown == true ){
        return ;
    }
    pool->pool_is_shutdown = true;
    pthread_mutex_lock(&pool->queue_lock);
    pthread_cond_broadcast(&pool->queue_ready);
    pthread_mutex_unlock(&pool->queue_lock);
    
    for(int index = 0;index<pool->max_pthread_num;++index){
        pthread_join(pool->thread_id[index],NULL);
    }
    free(pool->thread_id);
    while(pool->task_queue_head !=NULL){
        tmp_task = pool->task_queue_head;
        pool->task_queue_head = pool->task_queue_head->next;
        free(tmp_task);
    }
    pthread_mutex_destroy(&pool->queue_lock);
    pthread_cond_destroy(&pool->queue_ready);
    free(pool);
}

int add_task_to_threadpool(ThreadPool *pool,void*(*task_fun)(void*),void *args){
    Task *task;
    Task *tmp;
    if(task_fun == NULL){
        perror("task_fun is NULL !");
        return -1;
    }
    
    task =(Task *)malloc(sizeof(Task));
    if(task==NULL){
        perror("malloc task error");
        return -1;
    }
    task->work_func = task_fun;
    task->args=args;
    task->next=NULL;
    
    pthread_mutex_lock(&pool->queue_lock);
    tmp=pool->task_queue_head;
    if(tmp==NULL){
        pool->task_queue_head=task;
    }
    else{
        while(tmp->next!=NULL){
            tmp=tmp->next;
        }
        tmp->next = task;
    }
    
    pthread_mutex_unlock(&pool->queue_lock);
    pthread_cond_signal(&pool->queue_ready);
    return 0;
}
