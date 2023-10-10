#include<stdio.h>
#include<unistd.h>
int main(void)
{
	char buffer[24];
	int fd[2];
	pipe(fd);
	write(fd[1], "Hello, welcome to Pipe!!\n", 24);
	read(fd[0], buffer, sizeof(buffer));
	printf("The Pipe says = %s\n",buffer);
}
