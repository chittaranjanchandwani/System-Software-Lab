#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
int main(void)
{
	pid_t childpid=fork();
	if(childpid>0)
	{
		printf("In parent process : %d\n", getpid());
		return 0;
	}
	else
	{
		sleep(10);
		printf("Child pid=%d\n", getpid());
		printf("Parent pid=%d\n", getppid());
		printf("In child process\n");
		getchar();
		return 0;
	}
}
