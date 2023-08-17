#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
int main()
{
	const char*filename="my_file.txt";
	int fd=open(filename, O_RDWR | O_EXCL);
	if(fd==-1)
	{
		if(errno==EEXIST)
		{
			printf("File already exists. Opening in READWRITE mode.\n");
			fd=open(filename,O_RDWR);
			if(fd==-1)
			{
				printf("Error opening file");
				return 1;
			}
		}
		else
		{
			perror("Error opening file");
			return 1;
		}
	}
	else
	{ 	
		printf("File opened succesfully in read-write mode\n");
	}
	close(fd);
	return 0;
}

			
