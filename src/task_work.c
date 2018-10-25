#include "task_work.h"

#include "thread_pool.h"
#include <unistd.h>
#include<stdio.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<stdlib.h>


static void my_ll(const char *,const char* root);

void *task_cd(void *c){
	if(c==NULL){
		if(chdir($HOME)){
			return NULL;
		}
	}
	else{
		chdir((char*)c);	
	}
	return NULL;
}

void *task_ls(void *path){
	DIR 					*dp;
	struct dirent 			*dirp;

	if((dp=opendir((char*)path))==NULL){
		return strerror(2);
	}
	while((dirp=readdir(dp))!=NULL){
		my_ll(dirp->d_name,(char *)path);
	}
	return NULL;
}


void *task_pwd(void *c){
	char 				*buf;
	buf=(char*)malloc(sizeof(char)*PATH_MAX);
	memset(buf,0,PATH_MAX);
	return getcwd(buf,PATH_MAX); 
}

void *task_mkdir(void *c){
	umask(0);
	mkdir((char *)c,0755);
	return 0;
}

void *task_rmdir(void *c){
	
	 rmdir((char *)c);
	 return NULL;

}

static void my_ll(const char *name,const char *root){
	char path[128];
	struct stat buf;
	stat(name,&buf);
	memset(path,0,sizeof(path));
	sprintf(path,"%s%s%s",root,"/",name);

	printf("%3ld ",buf.st_nlink);
	printf("%5ld ",buf.st_size);
	printf("%10s",name);
	printf("\n");

}
