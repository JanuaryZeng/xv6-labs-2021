//
// Created by zxj on 2022/4/8.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(char* path, char* fname);
int cmp_path(char* path, char* key);
char* fmtname(char *path);

int
main(int agrc, char **agrv)
{
    if(agrc < 2){
        fprintf(2, "Usage: find args\n");
        exit(1);
    }
//    char *buf[512];
//    getcwd(buf, sizeof(buf));
//    fprintf(1,"%s\n",agrv[1]);
    find(agrv[1],agrv[2]);
    exit(0);
}

void
find(char* path, char* fname)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        switch(st.type){
            case T_FILE:
            {
                if(strcmp(de.name, fname) == 0){
                    fprintf(1, "%s\n", buf);
                }
                break;
            }
            case T_DIR:
            {
                if((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..") != 0)){
                    find(buf, fname);
                }
            }
        }
    }
}