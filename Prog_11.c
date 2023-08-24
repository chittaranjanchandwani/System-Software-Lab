#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
	int fd1, fd2, fd3, fd4;
	fd1=open("file1.txt",O_RDWR);
	fd2=dup(fd1);
	dup2(fd2,fd3);
	fd4=fcntl(fd1, F_DUPFD);
	printf("fd1=%d\n, fd2=%d\n, fd4=%d\n", fd1,fd2,fd4);
        return 0;
}	
