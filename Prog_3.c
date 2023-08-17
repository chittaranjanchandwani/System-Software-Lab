#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	const char* filename="my_file.txt";
	int fd=creat(filename,0644);
	if (fd==-1)
	{
		perror("Error creating file");
		return 1;
	}
	printf("FIle created successfully with file descriptor: %d\n", fd);
	close(fd);

	return 0;
}
