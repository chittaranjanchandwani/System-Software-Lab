#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
int main (int argc, char *argv[])
{
    struct stat fileStat;
    int fd=0;
    const char* filename = "file1.txt";

    fd = open(filename , O_RDONLY);
    if(fd==-1)
    {
        perror ( "open " );
        
    }

    if(fstat(fd, &fileStat)<0)
    {
	    return 1;
    }

    printf("Information for %s\n",filename);
    printf("File Size: \t\t%lu bytes\n",fileStat.st_size);
    printf("Number of Links: \t%lu\n",fileStat.st_nlink);
    printf("File inode: \t\t%lu\n",fileStat.st_ino);
    printf("FIle UID: \t\t%d\n",fileStat.st_uid);
    printf("File GID: \t\t%d\n",fileStat.st_gid);
    printf("File Block Size: \t\t%lu\n", fileStat.st_blksize);
    printf("Number of Blocks in FIle: \t\t%lu\n", fileStat.st_blocks);
    printf("Time of Last Access: \t\t%lu\n", fileStat.st_atim.tv_nsec);
    printf("TIme of Last Modification: \t\t%lu\n", fileStat.st_mtim.tv_nsec);
    printf("Time of Last Change: \t\t%lu\n", fileStat.st_ctim.tv_nsec);
    
    return 0;
}

