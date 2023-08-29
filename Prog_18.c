#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
int main(void)
{
        struct student
        {
                int RollNo;
		int Marks;
        }db;
        struct flock lock;
        int fd, newMarks,record;
        fd=open("filerecords.txt",O_RDWR);
        if (fd == -1) 
        {
          perror("Error opening the file");
          return 1;
        }
        printf("Select Record Number: 1, 2, 3\n");
        scanf("%d", &record);
        read(fd,&db,sizeof(db));
        lock.l_type=F_WRLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start = (record - 1) * sizeof(struct student);
        lock.l_len = sizeof(struct student);
        lock.l_pid = getpid();
        printf("Before entering the critical section\n");
        fcntl(fd,F_SETLKW, &lock);
	printf("Inside Critical Section");
	struct student s1;
        lseek(fd, (record - 1) * sizeof(struct student), SEEK_SET);
        read(fd, &s1, sizeof(struct student));
        printf("Current Marks: %d\n", s1.Marks);
        printf("Enter New Marks:");
	scanf("%d",&newMarks);
	s1.Marks=newMarks;
        lseek(fd, -1 * sizeof(struct student), SEEK_CUR);
        write(fd, &s1, sizeof(struct student));
        printf("To record new marks press enter\n");
        getchar();
        getchar();
        printf("Unlocked\n");        
        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK, &lock);
        printf("FINISH\n");
}

