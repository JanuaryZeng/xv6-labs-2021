//
// Created by zxj on 2022/4/8.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LEN 100

int
main(int argc, char* argv[])
{
    char *command = argv[1];
    char paramv[MAXARG][MAX_LEN];
    char *m[MAXARG];

    while(1){
        int result_num;
        char curr;
        int row_p = argc - 1;
        int col_p = 0;
        int pid;
        int flag = 0;

        if(row_p > MAXARG){
            fprintf(2, "Parameters are too many!\n");
            exit(1);
        }
        memset(paramv, 0, sizeof(paramv));
        for(int i = 1; i < argc; ++i){
            strcpy(paramv[i-1], argv[i]);
        }

        while((result_num = read( 0, &curr, 1)) > 0 && curr != '\n'){
            if(curr == ' ' && flag == 1){
                row_p++;
                col_p = 0;
                flag = 0;
                if(row_p > MAXARG){
                    fprintf(2, "Parameters are too many!\n");
                    exit(1);
                }
            }else if(curr != ' '){
                paramv[row_p][col_p++] = curr;
                flag = 1;
                if(row_p > MAX_LEN){
                    fprintf(2, "Parameters are too long!\n");
                    exit(1);
                }
            }
        }

        if(result_num <= 0)
            break;

        for(int i = 0; i < MAXARG; ++i){
            m[i] = paramv[i];
//            printf("%s\n", m[i]);
        }
//        printf("\n");
        m[MAXARG-1] = 0;
        if((pid = fork()) < 0){
            fprintf(2, "Failed to create child process!\n");
            exit(1);
        }else if(pid == 0){
            exec(command, m);
            exit(0);
        }else{
            wait(0);
        }
    }
    exit(0);
}