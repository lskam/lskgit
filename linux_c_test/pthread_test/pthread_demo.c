#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
#include <sys/signal.h>

sigset_t	mask; //信号集
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //信号锁
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;//条件变量

void* thread1(void* arg){
	printf("this is thread1,thread id is %u\n", (unsigned int)pthread_self());
	int err, signo;
	while(1)
	{
		printf("t1_0\n");
		err = sigwait(&mask, &signo);
		printf("t1_1\n");
		pthread_mutex_lock(&lock);
		printf("t1_2\n");
		pthread_cond_signal(&waitloc);//让线程2开始运行
		printf("t1_3\n");
		pthread_mutex_unlock(&lock);
		printf("t1_4\n");
	}
}
 void* thread2(void* arg){
	printf("this is thread2,thread id is %u\n", (unsigned int)pthread_self());
	while(1)
	{
		printf("t2_0\n");
		pthread_mutex_lock(&lock);
		printf("t2_1\n");
		pthread_cond_wait(&waitloc, &lock);//互斥锁+条件变量，使用此函数时，一定要先lock一把锁
		printf("t2_2\n");
		pthread_mutex_unlock(&lock);
		printf("t2_3\n");
	}
}
int main(void)
{
	printf("lsk thread demo\n");
	pthread_t tid1, tid2;
	int rc1=0, rc2=0,err = 0;
	sigset_t	oldmask;
	pthread_t	tid;
	/* 初始化信号相关操作 */
	sigemptyset(&mask);//将信号集置空
	sigaddset(&mask, SIGINT);//添加SIGINT信号到信号集mask中
	sigaddset(&mask, SIGQUIT);
	if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0) //屏蔽当前线程对所设置信号的响应，即按下CTRL+C程序不会退出
		err_exit(err, "SIG_BLOCK error");
		
	/* 创建两个线程 要在pthread_sigmask之后，否则pthread_sigmask不起作用。不知道为什么？？？*/
	rc1 = pthread_create(&tid1, NULL, thread1, NULL);
	if(rc1 != 0)
		printf("tid1 fail: %d\n",strerror(rc1));  
    rc1 = pthread_create(&tid2, NULL, thread2, &tid2);
	if(rc1 != 0)
		printf("tid2 fail: %d\n",strerror(rc1));  
	sleep(1);
	pthread_kill(tid1, SIGINT);//让线程1运行
	while(1){
		printf("t0_0\n");
		sleep(3);
	}
}