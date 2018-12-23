/* 示例二：主进程创建出来的线程将继承主进程的掩码
   来源：http://www.leoox.com/?p=321
   编译：gcc pthread_sigmask2.c -lpthread

	运行后，你可以发现，子线程果然接收不到pthread_kill发送给自己的SIGINT信号，
	但可以收到SIGUSR1信号！本来要休眠300s，但是收到了SIGUSR1信号，
	才休眠5秒就（提前294秒）醒来say goodbye了。
	运行结果：
	[lgh@lghvm001 thread]$ ./a.out 
	[main][139999919077120] working hard ...
	>>> Thread[139999919068928] Running ......
	[main][139999919077120] send signal SIGINT ...
	Thread[139999919068928] catch signo = 10
	Thread[139999919068928] waitup(294), and say good bye!
	[main][139999919077120] good bye and good luck!
	[lgh@lghvm001 thread]$ 

*/



#include <pthread.h>
#include <stdio.h>
#include <sys/signal.h>
#include <string.h>

void handler(int signo)
{
  pthread_t tid = pthread_self();
  printf("Thread[%lu] catch signo = %d\n", tid, signo);
  return;
}

void* run(void *param)
{
  pthread_t tid = pthread_self();
  printf(">>> Thread[%lu] Running ......\n", tid);

  int rc = sleep(300);
  printf("Thread[%lu] waitup(%d), and say good bye!\n", tid, rc);
  return NULL;
}

int main(int argc, char** argv)
{
  int ret = 0, i = 0;
  pthread_t tid = pthread_self();
  
  /* 注册SIGUSR1信号处理函数 */
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  sigaction(SIGUSR1, &sa, NULL);

  /* 屏蔽信号SIGINT */
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  pthread_sigmask(SIG_BLOCK, &mask, NULL);

  pthread_t threads[2];
  pthread_create(&threads[0], NULL, run, NULL);
  printf("[main][%lu] working hard ...\n", tid);
  sleep(1);

  /* 主进程创建出来的线程将继承主进程的掩码。所以子线程收不到SIGINT信号。 */
  pthread_kill(threads[0], SIGINT);
  printf("[main][%lu] send signal SIGINT ...\n", tid);
  sleep(5);

  /* 子线程可以收到SIGUSR1信号。 */
  pthread_kill(threads[0], SIGUSR1);

  pthread_join(threads[0], NULL);

  sleep(1);
  printf("[main][%lu] good bye and good luck!\n", tid);
  return 0;
}
