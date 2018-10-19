#include "thread_pool.h"
#include "condition.h"
#include "socket_init.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void * handle_accept(void* arg) ;

int main(){
	int 		listenfd;
	listenfd=socket_bind("127.0.0.1",20000);
	listen(listenfd,10);
	thread_pool_t *pool;
	pool=threadpool_init(10,10);
	threadpool_add_task(pool,handle_accept,(void*)&listenfd);
	for(;;);
	return 0;

}

void * handle_accept(void *arg){
 	int 					*fd=(int*)arg;
	int 					connectfd;
	struct sockaddr_in 		addr;
	socklen_t						len;
	connectfd=accept(*fd,(struct sockaddr*)&addr,&len);
	if(connectfd==-1){
		perror("accept error");
	}
	else {
		printf("accept a new client :%s:%d\n",inet_ntoa(addr.sin_addr),addr.sin_port);
	}
	return NULL;
}
