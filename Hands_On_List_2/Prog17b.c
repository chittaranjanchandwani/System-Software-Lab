#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
int main()
{
	int fd[2], p=pipe(fd);
	if(p==-1)
	{
		perror("pipe");
		return 1;
	}
	if(fork()==0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execlp("ls", "ls", "-l", NULL);
		perror("execlp");
		return 1;
	}
	else
	{
		close(fd[1]);
		if(fork()==0)
		{
			dup2(fd[0], STDIN_FILENO);
			execlp("wc","wc", NULL);
			perror("execlp");
			return 1;
		}
		else
		{
			wait(NULL);
			wait(NULL);
		}
	}
	return 0;
}
