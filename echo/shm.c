#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    key_t key;
    int shmid;
    char *p;

    key = ftok("/", 'a');
    shmid = shmget(key, 128, IPC_CREAT | IPC_EXCL | 0664);

    if(shmid == -1) {
        if(errno == EEXIST) {
            printf("共享内存已被创建");
            shmid = shmget(key, 128, 0664);
        } else {
            perror("shmget\n");
            return -1;
        }
    } else {
        printf("创建成功!\n");
    }

    p = shmat(shmid, NULL, 0);
    strcpy(p, "hello world\n");
    printf("buff:%s\n", p);
    shmdt(p);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
