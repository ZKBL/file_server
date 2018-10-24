#include "do_epoll.h"
#include "task_work.h"
#include "thread_pool.h"

#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int do_epoll(int fd){
	int 					epollfd;
	int 					ret;
	struct epoll_event		events[EPOLLEVENTS];
	epollfd=epoll_create(FDSIZE);
	add_event(epollfd,fd,EPOLLIN|EPOLLONESHOT);

	thread_pool_t *pool;
	pool=threadpool_init(10,10);

	for(;;){
		ret=epoll_wait(epollfd,events,EPOLLEVENTS,-1);	
		handle_event(epollfd,events,ret,fd,pool);
	}
	return 0;
}

int handle_event(int epollfd,struct epoll_event *events,int num,int listenfd,thread_pool_t *pool){
	int 					i;
	int 					fd;
	char 					*buf;
	comment_t 				comment;
	buf=(char*)malloc(MAXSIZE);
	for(i=0;i<num;i++){
		fd=events[i].data.fd;
		if((fd==listenfd)&&(events[i].events&EPOLLIN))
			handle_accept(epollfd,listenfd);
		else if(events[i].events&EPOLLIN)
			do_read(epollfd,fd,&comment,pool);
		else if(events[i].events&EPOLLOUT)
			do_write(epollfd,fd,buf);

	}
	return -1;
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
} void modify_event(int epollfd,int listenfd,int state){	
	struct epoll_event ev;
	ev.events=state;
	ev.data.fd=listenfd;
	epoll_ctl(epollfd,EPOLL_CTL_MOD,listenfd,&ev);
}

void do_read(int epollfd,int listenfd,comment_t *comment,thread_pool_t *pool){
	int 					nread;
	char 					buf[MAXSIZE];
	nread=read(listenfd,buf,MAXSIZE);
	if(nread==-1){
		perror("read error");
		close(listenfd);
		delete_event(epollfd,listenfd,EPOLLIN);
	}
	else if(nread==0){
		printf("client close\n");
		close(listenfd);
		delete_event(epollfd,listenfd,EPOLLIN);
	}
	else {
		printf("read buf is %s\n",buf);
		modify_event(epollfd,listenfd,EPOLLOUT);
		memcpy(comment,buf,sizeof(comment_t));
		printf("comment.arg=%s\n",(char*)(comment->arg));
		threadpool_add_task(pool,task_ls,comment->arg);
		delete_event(epollfd,listenfd,EPOLLOUT);
		//handle_comment(buf,pool);
	}

}
void do_write(int epollfd,int listenfd,char *buf){
	int 					nwrite;
	strcpy(buf,"hello");
	nwrite=write(listenfd,buf,MAXSIZE);
	if(nwrite==-1){
		perror("write error");
		close(listenfd);
		delete_event(epollfd,listenfd,EPOLLOUT);
	}
	else{
		modify_event(epollfd,listenfd,EPOLLIN);
		memset(buf,0,MAXSIZE);
	}

}
void handle_comment(char *buf,thread_pool_t *pool){
	
	comment_t 					commen;
	memcpy(&commen,buf,sizeof(comment_t));
	int b=commen.comment;
	switch(b){
		
		case LS:
		threadpool_add_task(pool,task_ls,commen.arg);
		case CD:
			threadpool_add_task(pool,task_cd,commen.arg);
		case PWD:
			threadpool_add_task(pool,task_pwd,commen.arg);
	}
}

