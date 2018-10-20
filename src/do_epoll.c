#include "do_epoll.h"
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>

int handle_accept(int epollfd,int listenfd){
	int 					new_fd;
	struct sockaddr_in		clientaddr;
	socklen_t 				len;
	
	new_fd=accept(listenfd,(struct sockaddr *)clientaddr,&len);
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
	return new_fd;
}
