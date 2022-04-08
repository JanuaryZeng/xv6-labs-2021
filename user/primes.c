#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ         0
#define WRITE        1
#define PRIME_NUM   35

void child(int *pl);

int
main(void)
{
    int p[2];
    int pid;

    if(pipe(p) != 0){
        fprintf(2, "pip create failed!\n");
        exit(1);
    }

    if((pid = fork()) < 0) {
        fprintf(2, "create child process failed!\n");
        exit(1);
    }else if(pid == 0){
        child(p);
    }else{
        close(p[READ]);
        for(int i = 2; i <= PRIME_NUM; ++i){
            write(p[WRITE], &i, sizeof(i));
        }
        close(p[WRITE]);
        wait((int*)0);
    }
    exit(0);
}

void
child(int *pl)
{
    int pr[2];
    int n;
    int pid;

    close(pl[WRITE]);

    if(read(pl[READ], &n, sizeof(n)) == 0)
        exit(0);

    if(pipe(pr) != 0){
        fprintf(2, "pip create failed!\n");
        exit(1);
    }

    if((pid = fork()) < 0){
        fprintf(2, "create child process failed!\n");
        exit(1);
    }else if(pid == 0){
        child(pr);
    }else{
        close(pr[READ]);
        fprintf(1, "prime %d\n", n);
        int prime = n;
        while(read(pl[READ], &n, sizeof(n)) != 0){
            if(n % prime == 0){
                continue;
            }
            write(pr[WRITE], &n, sizeof(n));
        }
//        fprintf(1, ">%d\n", getpid());
        close(pr[WRITE]);
        close(pl[READ]);
        wait((int*)0);
    }
    exit(0);
}