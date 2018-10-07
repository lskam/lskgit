#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<sys/sem.h>
#include<string.h>
#include<sys/stat.h>
#define SEM_KEY 6666
#define MY_SHM_KEY 2400
union semun {
	int setval;
	struct semid_ds *buf;
	unsigned short *array;
};

int main(int argc ,char *argv[])
{
	int	shmid;
	float *addr;
	float h,w; 
	shmid= shmget(MY_SHM_KEY,getpagesize(), IPC_CREAT | 0666);//打开刚才原程序1创建的共享内存，权限为0666
	printf("shmid=%d\n",shmid);
	if(shmid==-1)
	{
	perror("shmget error:");
	exit(EXIT_FAILURE);
	}
	addr =shmat(shmid,0,0);//获取共享内存的起始地址，且为可读可写
	if(-1== *addr)
	{
		perror("shmat error:");
		exit(EXIT_FAILURE);
	}
	
	int semid;
	int ret;
	semid = semget(SEM_KEY, 0, IPC_CREAT | 0600);//取得信号量
	if (-1 == semid)
	{
		perror("semget");
		exit(EXIT_FAILURE);
	}
	printf("semid = %d\n", semid);
		
//对资源的使用处理操作
	struct sembuf sem_opt_wait1[1] = {0, -1, SEM_UNDO};
	struct sembuf sem_opt_wakeup1[1] = {0, 1, SEM_UNDO};
	struct sembuf sem_opt_wait2[1] = {1, -1, SEM_UNDO};
	struct sembuf sem_opt_wakeup2[1] = {1, 1, SEM_UNDO};
	while(1)
	{
		printf("t1\n");
		semop(semid, sem_opt_wait1, 1);//获取进程1的资源好让进程1等待。
		h=*addr;
		w=*(addr+1);
		printf("h=%f\tw=%f\n",h,w);
		if(h==1.0 || w==1.0) break;;//如果输入身高是1或体重是1就退出
		int ret=w/(h*h/10000);
		if(ret>=20 &&ret <=25)
		{
			printf("ok!\n");
		}
		else if(ret <20)
		{
			printf("thin!\n");
		
		} 
		else
		{
			printf("fat!\n");
		}

		printf("t2\n");
		semop(semid, sem_opt_wakeup2, 1);//释放进程2的资源，即唤醒进程2	
		printf("t3\n");
	}
		if(-1==semctl(semid,1,IPC_RMID,0))//删除信号量
		{
		perror("semctl error:");
		exit(EXIT_FAILURE);
		}
		if(-1==shmdt(addr))//释放共享内存，使其不再有任何指向它的指针
		{
		perror("shmdt error:");
		exit(EXIT_FAILURE);
		
		}
		if (shmctl(shmid,IPC_RMID,0)==-1)//删除共享内存
		{
		perror("shctl error:");
		exit(EXIT_FAILURE);
		}
	return 0;
}
