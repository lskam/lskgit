/* 示例一：屏蔽信号SIGINT
   来源：http://www.leoox.com/?p=321
   编译：gcc pthread_sigmask1.c -lpthread

   运行后，你发现你按下CTRL+C(触发SIGINT信号)，这个程序根本停不下来。因为SIGINT信号已经如我们所愿被屏蔽掉了。
*/

#include <pthread.h>
#include <stdio.h>
#include <sys/signal.h>
#include <string.h>

int main(int argc, char** argv)
{
	//pthread_t tid = pthread_self();
	sigset_t mask,oldmask;
	int err;
#if 0
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	pthread_sigmask(SIG_SETMASK, &mask, NULL);//SIG_BLOCK SIG_SETMASK 会屏蔽掉SIGINT，但SIG_UNBLOCK不会屏蔽SIGINT

	printf("[main][%lu] working hard ...\n", tid);

	sleep(60);

	printf("[main][%lu] good bye and good luck!\n", tid);
#else
	
	/* 初始化信号相关操作 */
	sigemptyset(&mask);//将信号集置空
	sigaddset(&mask, SIGINT);//添加SIGINT信号到信号集mask中
	sigaddset(&mask, SIGQUIT);
	if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0) //屏蔽当前线程对所设置信号的响应，即按下CTRL+C程序不会退出
		err_exit(err, "SIG_BLOCK error");
	while(1){
		printf("t0_0\n");
		//err = sigwait(&mask, &signo);
		sleep(3);
	}	
#endif
	return 0;
}
