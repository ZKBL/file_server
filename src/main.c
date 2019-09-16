//
//  main.c
//  file_server
//
//  Created by 冯蕗铭 on 2019/9/11.
//  Copyright © 2019 冯蕗铭. All rights reserved.
//

#include "thread_pool.h"
#include <unistd.h>
#include <stdio.h>

void * func(void * args){
    int num= (int)args;
    printf("threadID : %lx printf number: %d \n",(unsigned long)pthread_self(),num);
    return NULL;
}

int main(int argc, const char * argv[]) {
    
    ThreadPool *pool = NULL;
    if(0 != create_pool(&pool, 10)){
        printf("create pool failed ! \n");
        return -1;
    }
    for( int i=0; i<1000;i++){
        add_task_to_threadpool(pool, func,  (void *)i);
    }
    sleep(2);
    destory_pool(pool);
    return 0;
}
