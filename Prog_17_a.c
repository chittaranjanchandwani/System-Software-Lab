#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
int main(void)
{
        struct
        {
                int ticketNumber;
        }db;
        struct flock lock;
        int fd;
        fd=open("fileticket.txt",O_RDWR);
        read(fd,&db,sizeof(db));
        lock.l_type=F_WRLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=0;
        lock.l_len=0;
        lock.l_pid=getpid();
        printf("Before entering the critical section\n");
        fcntl(fd,F_SETLKW, &lock);
        printf("Current Ticket Number= %d\n",db.ticketNumber);
        db.ticketNumber++;
        lseek(fd,0L,SEEK_SET);
        printf("Inside Critical Section\n");
        printf("Press enter to unlock\n");
        write(fd,&db, sizeof(db));
        getchar();
        lock.l_type=F_UNLCK;
        printf("Unlocked\n");
        fcntl(fd,F_SETLK, &lock);
        printf("FINISH\n");
}
