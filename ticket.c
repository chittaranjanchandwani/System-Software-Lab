#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
int main(void)
{
	struct
	{
		int ticketno;
	}db;
	int fd;
	fd=open("fileticket.txt", O_CREAT | O_RDWR, 0744);
	printf("Enter Ticket Number:");
	scanf("%d",&db.ticketno);
	write(fd, &db, sizeof(db));
	getchar();

}

