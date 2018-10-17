#include "do_comment.h"
#include <unistd.h>
#include<stdio.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>


void my_ll(const char *,const char* root);

void *comment_cd(void *c){
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

void *comment_ls(void *path){
	DIR 					*dp;
	struct dirent 			*dirp;

	if((dp=opendir(argv[1]))==NULL){
		return ;
	}
	while((dirp=readdir(dp))!=NULL){
		my_ll(dirp->d_name,(char *)path);
	}
	return NULL;
}


void *comment_pwd(void *c){


	return NULL; 

}

void my_ll(const char *name,const char *root){
	char path[128];
	struct stat buf;
	stat(name,&buf);
	memset(path,0,sizeof(path));
	sprintf(path,"%s%s%s",root,"/",name);

	printf("%3ld ",buf.st_nlink);
	printf("%6s ",getpwuid(buf.st_uid)->pw_name);
	printf("%6s ",getgrgid(buf.st_gid)->gr_name);
	printf("%5ld ",buf.st_size);
	printf("%10s",name);
	printf("\n");

}
