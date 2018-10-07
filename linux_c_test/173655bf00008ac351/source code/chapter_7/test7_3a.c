#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 4096
#define MYKEY 2400

int main(void)
{
    int shmid;
    char *shmptr;
	shmid=shmget(MYKEY,BUF_SIZE,IPC_CREAT);
	printf("test:%d\r\n",shmid);
    if(shmid==-1)
    {
        printf("shmget error!\n");
        exit(1);
    }
    if((shmptr=shmat(shmid,0,0))==( void*)-1)
    {
        fprintf(stderr,"shmat error!\n");
        exit(1);
    }
    while(1)
    {
        printf("string:%s\n",shmptr);

        sleep(3);
    }
    exit(0);
}

