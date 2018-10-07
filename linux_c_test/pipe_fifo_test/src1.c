#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<limits.h>
int main()
{
    const char *fifo_name = "/tmp/my_fifo";
   	int tem_value;
	 int pipe_fd = -1;
    int data_fd = -1;
    int res = 0;
    const int open_mode = O_WRONLY;
    char buffer[PIPE_BUF+1];
    if(access(fifo_name,F_OK)==-1) 
    {
        res = mkfifo(fifo_name,0777);//创建命名管道文件
        if(res!=0)
        {
            fprintf(stderr,"could not create fifo\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("process %d opening fifo O_WRONLY\n",getpid());
	printf("input to open\n");
	scanf("%d",&tem_value);
    pipe_fd = open(fifo_name,open_mode);//打开写管道
	printf("pipe_fd:%d\n",pipe_fd);
    data_fd = open("data.txt",O_RDONLY);//打开文件
    printf("process %d result %d\n",getpid(),pipe_fd);
    if(pipe_fd!=-1)
    {
        int bytes_read = 0;
        bytes_read = read(data_fd,buffer,PIPE_BUF);//读文件数据
		printf("bytes_read:%d\n",bytes_read);
        while(bytes_read>0)
        {
			printf("input to pipe write\n");
			scanf("%d",&tem_value);
            res = write(pipe_fd,buffer,bytes_read);//将读到的数据通过管道写入
           printf("res:%d\n",res);
			 if(res==-1)
            {
                fprintf(stderr,"write error\n");
                exit(EXIT_FAILURE);
            }
            bytes_read = read(data_fd,buffer,PIPE_BUF);//读文件数据
			printf("bytes_read:%d\n",bytes_read);
            buffer[bytes_read]='\0';
        }
        close(pipe_fd);
        close(data_fd);
    }
    else{
        exit(EXIT_FAILURE);
    }
    printf("process %d finished.\n",getpid());
    exit(EXIT_SUCCESS);
}

