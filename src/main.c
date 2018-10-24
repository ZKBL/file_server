#include "thread_pool.h"
#include "condition.h"
#include "socket_init.h"
#include "do_epoll.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main(){
	int 		listenfd;
	listenfd=socket_bind("127.0.0.1",20000);
	listen(listenfd,10);
	thread_pool_t *mainreactor;
	thread_pool_t *pool;
	pool=threadpool_init(10,10);
	mainreactor=threadpool_init(1,10);
	do_epoll(listenfd);
	return 0;
}
