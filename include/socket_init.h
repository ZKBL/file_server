#ifndef __SOCKET_INIT_H__
#define __SOCKET_INIT_H__

#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int socket_bind(const char *,int );

#endif
