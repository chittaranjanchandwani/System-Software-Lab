#include<stdio.h>
#include<unistd.h>
int main(void)
{
	pid_t pid;
	printf("Process ID of the program is= %d\n",getpid());
        if(!fork())
	{
		printf("In child process. The process ID of child is=%d\n and its parent process ID is = %d\n",getpid(), getppid());
	}
	else{
		printf("In parent process. The process ID of parent is=%d\n", getpid());
	}

	return 0;
}	
