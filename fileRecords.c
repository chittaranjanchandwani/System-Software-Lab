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
		int RollNo;
		int Marks;
	}db[3];
	int fd, i=0;
	for( i=0; i<3; i++)
	{
		printf("Enter Roll  Number:");
		scanf("%d",&db[i].RollNo);
		printf("\nEnter Marks:");
		scanf("%d",&db[i].Marks);
		getchar();
	}

	fd=open("filerecords.txt", O_CREAT | O_RDWR, 0744);
	if (fd == -1)
	{
          perror("Error opening the file");
          return 1;
        }
        ssize_t fd_write = write(fd, db, sizeof(db));
        if (fd_write == -1) 
        {
          perror("Error writing into the file");
          close(fd);
          return 1;
        }

        int fd_close = close(fd);
        if (fd_close == -1) 
        {
        perror("Error closing the file");
        return 1;
        }

	

}

