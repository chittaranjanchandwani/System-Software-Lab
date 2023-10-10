#include<stdio.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	int fd[2], p=pipe(fd);
	if(p==-1)
	{
		perror("pipe");
		return 1;
	}
	pid_t lsid, wcid;
	lsid=fork();
	if(lsid==-1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(lsid==0)
	{
		close(fd[0]);
		int output=dup(fd[1]);
		if(output==-1)
		{
			perror("dup");
			exit(EXIT_FAILURE);
		}
		close(fd[1]);
		if(output!=STDOUT_FILENO)
		{
			if(dup2(output,STDOUT_FILENO)==-1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(output);
		}
		execlp("ls","ls","-l", NULL);
		perror("execlp");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(fd[1]);
		wcid=fork();
		if(wcid==-1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if(wcid==0)
		{
			int input=dup(fd[0]);
			if(input==-1)
			{
				perror("dup");
				exit(EXIT_FAILURE);
			}
			close(fd[0]);
			if(input!=STDIN_FILENO)
			{
				if(dup2(input, STDIN_FILENO)==-1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
				close(input);
			}
			execlp("wc","wc",NULL);
			perror("execlp");
			exit(EXIT_FAILURE);
		}
		else
		{
			close(fd[0]);
			wait(NULL);
			wait(NULL);
		}
	}
	return 0;
}

