#include<unistd.h>
#include<fcntl.h>
int main(void)
{
	int fd1, fd2, i;
	char buffer[1024];
	fd1=open("file1.txt", O_RDONLY);
	i=read(fd1,buffer, sizeof(buffer));
	fd2=open("file2.txt", O_WRONLY);
	write(fd2,buffer,i);
	close(fd1);
	close(fd2);
}

