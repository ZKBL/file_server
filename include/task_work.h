#ifndef __DO_COMMENT__H_
#define __DO_COMMENT__H_ 

#define $HOME "/home/clown/"
typedef	enum{
		LS,CD,PWD,UPLOAD,DOWNLOAD,REMOVE,
	}comment_u;

typedef struct {
	comment_u comment;
	void *arg;
}comment_t;

void *task_cd(void *);
void *task_ls(void *);
void *task_pwd(void *);
void *task_mkdir(void *);
void *task_rmdir(void *);
void *task_down(void *);
void *task_up(void *);
void *task_rm(void *);


#endif
