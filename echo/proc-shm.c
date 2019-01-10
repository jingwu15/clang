/*
练习：
    程序1(写)：创建共享内存成功，代表先执行；映射，写入自己的pid；
                    等待对方读取pid并且写入它的pid
                直到接收到对方的信号，读取对方的pid..

                创建共享内存的失败(已存在)；代表后执行；
                    打开并且映射，直接读取对方的pid，写入自己的pid;
                        发送信号告诉对方
    程序2(读)：
写：写入数据，发送信号告诉对方，等待对方读取数据；
读：等待对方写入数据，读取数据，发送信号告知对方数据已读取；
直到输入的为"quit"时，程序结束，删除共享内存。
*/



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void ignore(int sig)
{
    ;
}

typedef struct shm {
    pid_t wpid;
    pid_t rpid;
    char data[32];
} shm_t;

int main(int argc, const char *argv[])
{
    signal(SIGUSR1, ignore);
    key_t key;
    int shmid, flag;
    flag = 0;
    shm_t *myshm;

    key = ftok("/tmp/shm-l1.shm", 'a');
    shmid = shmget(key, sizeof(shm_t), IPC_CREAT | IPC_EXCL | 0664);
    if(shmid == -1) {
        if(errno == EEXIST) {
            flag =  1;
            printf("共享内存已被创建\n");
            shmid = shmget(key, sizeof(shm_t), 0664);
        } else {
            perror("shmget\n");
            return -1;
        }
    } else {
        printf("创建成功!\n");
    }
    myshm = shmat(shmid, NULL, 0);
    if(flag == 0) {
        //写进程
        myshm->wpid = getpid();
        printf("写进程[%d]创建成功，等待读\n", myshm->wpid);
        pause();
        printf("读进程[%d]已经启动\n", myshm->rpid);
    } else {
        //读进程
        myshm->rpid = getpid();
        printf("这里是读进程[%d]\n", myshm->rpid);

        printf("写进程[%d]已启动\n", myshm->wpid);
    }
    while(1) {
        if(flag == 0) {
            scanf("%s", myshm->data);
            kill(myshm->rpid, SIGUSR1);
        } else {
            printf("buff[%d]:%s\n", strlen(myshm->data), myshm->data);
            kill(myshm->wpid, SIGUSR1);
        }
        if(strncmp(myshm->data, "quit", 4) == 0) {
            return 0;
        } else {
            pause();
        }
    }

    shmdt(myshm);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

