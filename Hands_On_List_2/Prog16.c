#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
	char buffer1[50],buffer2[50];
	int fd1[2],fd2[2];
	pipe(fd1);
	pipe(fd2);
	if(!fork())
	{
		close(fd1[0]);
		printf("Enter Message for Parent = ");
		scanf("%[^\n]",buffer1);
		write(fd1[1], buffer1, sizeof(buffer1));
		close(fd2[1]);
		read(fd2[0], buffer2, sizeof(buffer2));
		printf("Message from parent = %s\n",buffer2);
	}
	else
	{
		close(fd1[1]);
                read(fd1[0], buffer1, sizeof(buffer1));
                printf("Message from child  = %s\n",buffer1);
		close(fd2[0]);
		printf("Enter Message for Child = ");
		scanf("%[^\n]",buffer2);
		write(fd2[1], buffer2, sizeof(buffer2));
	}
	return 0;
}

