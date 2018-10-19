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

void *comment_cd(void *);
void *comment_ls(void *);
void *comment_pwd(void *);
void *comment_mkdir(void *);
void *comment_rmdir(void *);
void *comment_down(void *);
void *comment_up(void *);
void *comment_rm(void *);

#endif
