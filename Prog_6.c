#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	char buffer[1024];
	ssize_t Amt_bytes_read;
	while((Amt_bytes_read=read(STDIN_FILENO, buffer, sizeof(buffer)))>0)
	{
		write(STDOUT_FILENO, buffer, Amt_bytes_read);
	}
	if(Amt_bytes_read==-1)
	{
		perror("Issue in reading input");
		return 1;
	}

	return 0;
}
