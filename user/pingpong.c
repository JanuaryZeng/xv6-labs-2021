#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ   0
#define WRITE  1

int
main(int argc, char*argv[])
{
    int pip_p2c[2];
    int pip_c2p[2];
    if(pipe(pip_p2c) != 0){
        fprintf(2, "pipe syscall failed!\n");
        exit(1);
    }
    if(pipe(pip_c2p) != 0){
        fprintf(2, "pipe syscall failed!\n");
        exit(1);
    }
    int pid;
    if((pid = fork()) < 0){
        fprintf(2, "create child process failed!\n");
        exit(1);
    }else if(pid == 0){
        //子进程
        char rec;
        close(pip_p2c[WRITE]);
        close(pip_c2p[READ]);
        read(pip_p2c[READ], &rec, sizeof(rec));
        fprintf(1, "%d: received ping\n", getpid());
        write(pip_c2p[WRITE], &rec, sizeof(rec));
        close(pip_p2c[READ]);
        close(pip_c2p[WRITE]);
    }else{
        //父进程
        char send = 'q';
        char rec;
        close(pip_p2c[READ]);
        close(pip_c2p[WRITE]);
        write(pip_p2c[WRITE], &send, sizeof(send));
        read(pip_c2p[READ], &rec, sizeof(rec));
        fprintf(1, "%d: received pong\n", getpid());
        close(pip_p2c[WRITE]);
        close(pip_c2p[READ]);
        wait(0);
    }

    exit(0);
}