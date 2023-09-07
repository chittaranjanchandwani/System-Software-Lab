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
		sleep(50);
	}
	else
	{
		printf("Child executed successfully");
	}
	return 0;
}
