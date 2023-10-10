#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
	char buffer[50];
	int fd[2];
	pipe(fd);
	if(fork())
	{
		close(fd[0]);
		printf("Enter Message for Child = ");
		scanf("%[^\n]",buffer);
		write(fd[1], buffer, sizeof(buffer));
	}
	else
	{
		close(fd[1]);
		read(fd[0], buffer, sizeof(buffer));
		printf("Message from parent = %s\n",buffer);
	}
	return 0;
}


