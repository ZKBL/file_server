#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIR_LENGTH  255

int main(int args,char ** argc){
    char  * path ;
    char **file_name;

    if(args == 1){
        path   = "./";
    }
    else if (args == 2){
        path= argc[1];
    }
    else {
        printf("usage : ls  dirctory_name \n");
        return -1;
    }
    DIR             *dp;
    struct dirent   *dirp;
    if((dp = opendir(path)) == NULL){
        perror("open dir failed");
    }
    int length=0;
    while(( dirp = readdir(dp)) != NULL){
        file_name[length]=(char*)malloc(sizeof(char *));
        file_name[length++]=dirp->d_name;
        printf("%s      ",dirp->d_name);
    }
    printf("\n");
    printf("___________________________\n");
    for(int index=0;index<length;index++){
        printf("%s\n",file_name[index]);
    }
    return 0;
}