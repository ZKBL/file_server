#ifndef __DO_EPOLL_H_
#define __DO_EPOLL_H_
#include <sys/epoll.h>
//IO多路复用EPOLL
int do_epoll(int fd);
// 事件处理函数
int handle_events(int epollfd,struct epoll_events *events,int num,int listenfd);
// 连接处理函数 accept接受链接，并且将链接绑定在epollfd上
int handle_accept(int epollfd,int listenfd);
//
#endif
