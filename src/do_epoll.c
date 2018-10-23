#include "do_epoll.h"
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>

int do_epoll(int fd){
	int 					epollfd;
	struct epoll_event		events[EPOLLEVENTS];
	epollfd=epoll_create(FDSIZE);
	add_event(epollfd,fd,EPOLLIN);
	for(;;){
		epoll_wait(epollfd,events,EPOLLEVENTS,-1);	
		handle_accept(epollfd,fd);
	}


	return epollfd;
}

int handle_accept(int epollfd,int listenfd){
	int 					new_fd;
	struct sockaddr_in		clientaddr;
	socklen_t 				len;
	
	new_fd=accept(listenfd,(struct sockaddr *)&clientaddr,&len);
	if(new_fd==-1){
		perror("accept error");
		return -1;
	}
	else{
		int 				oldflag,newflag;
		if((oldflag=fcntl(new_fd,F_GETFL,0)<0)){
			perror("F_GETFL error");
			return -2;
		}
		else{
			newflag=oldflag | O_NONBLOCK;
			if(fcntl(new_fd,F_SETFL,newflag)==-1){
				perror ("F_SETFL error");
				return -3;
			}
		
		}
		
	}
	add_event(epollfd,new_fd,EPOLLIN);
	printf("hello,world\n");
	return new_fd;
}
void add_event(int epollfd,int listenfd,int state){	
	struct epoll_event ev;
	ev.events=state;
	ev.data.fd=listenfd;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev);
}
void delete_event(int epollfd,int listenfd,int state){	
	struct epoll_event ev;
	ev.events=state;
	ev.data.fd=listenfd;
	epoll_ctl(epollfd,EPOLL_CTL_DEL,listenfd,&ev);
}
void modify_event(int epollfd,int listenfd,int state){	
	struct epoll_event ev;
	ev.events=state;
	ev.data.fd=listenfd;
	epoll_ctl(epollfd,EPOLL_CTL_MOD,listenfd,&ev);
}
