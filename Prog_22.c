#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
int main(void)
{
	int fd;
	char buff_1[]={"The parent is writing in the file.\n"};
	char buff_2[]={"The child is writing in the file.\n"};
	fd=open("file22.txt", O_WRONLY);
	if(fd==-1)
	{
		printf("Error opening file");
	}
        if(!fork())
	{
		write(fd,buff_1,sizeof(buff_1));
	}
	else
	{
		write(fd,buff_2,sizeof(buff_2));
	}
	close(fd);

	return 0;
}	
