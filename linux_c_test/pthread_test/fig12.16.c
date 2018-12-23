#include "apue.h"
#include <pthread.h>

int			quitflag;	/* set nonzero by thread */
sigset_t	mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void *
thr_fn(void *arg)
{
	int err, signo;

	for (;;) {
		printf("t1\n");
		err = sigwait(&mask, &signo);
		printf("t1_1\n");
		if (err != 0)
			err_exit(err, "sigwait failed");
		switch (signo) {
		case SIGINT:
			printf("\ninterrupt\n");
			break;

		case SIGQUIT:
			printf("t2\n");
			pthread_mutex_lock(&lock);
			quitflag = 1;
			printf("t2_1\n");
			pthread_mutex_unlock(&lock);
			printf("t2_2\n");
			pthread_cond_signal(&waitloc);//解除主线程条件等待
			printf("t2_3\n");
			return(0);

		default:
			printf("unexpected signal %d\n", signo);
			exit(1);
		}
	}
}

int
main(void)
{
	int			err;
	sigset_t	oldmask;
	pthread_t	tid;

	sigemptyset(&mask);//将信号集置空
	sigaddset(&mask, SIGINT);//添加SIGINT信号到信号集mask中
	sigaddset(&mask, SIGQUIT);
	printf("lsk test\n");
	if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0) //屏蔽当前线程对所设置信号的响应
		err_exit(err, "SIG_BLOCK error");

	err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err != 0)
		err_exit(err, "can't create thread");

	pthread_mutex_lock(&lock);
	printf("t0:%d\n",quitflag);
	while (quitflag == 0){
		printf("t0_0\n");
		pthread_cond_wait(&waitloc, &lock);
		printf("t0_1\n");
	}
	pthread_mutex_unlock(&lock);
	printf("t0_2\n");
	/* SIGQUIT has been caught and is now blocked; do whatever */
	quitflag = 0;

	/* reset signal mask which unblocks SIGQUIT */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");
	exit(0);
}
