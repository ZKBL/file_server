#ifndef __DO_EPOLL_H_
#define __DO_EPOLL_H_

#include <sys/epoll.h>

#include "thread_pool.h"
#include "task_work.h"

#define EPOLLEVENTS 10
#define FDSIZE 		10
#define MAXSIZE     1024

//IO多路复用EPOLL
int do_epoll(int fd);
// 事件处理函数
int handle_event(int epollfd,struct epoll_event *events,int num,int listenfd,thread_pool_t *pool);
// 连接处理函数 accept接受链接，并且将链接绑定在epollfd上
int handle_accept(int epollfd,int listenfd);
void handle_comment(char *buf,thread_pool_t *pool);
// add event
void add_event(int epollfd,int listenfd,int state);
void delete_event(int epollfd,int listenfd,int state);
void modify_event(int epollfd,int listenfd,int state);

void do_read(int epollfd,int listenfd,comment_t *,thread_pool_t *pool);
void do_write(int epollfd,int listenfd,char *buf);
#endif
