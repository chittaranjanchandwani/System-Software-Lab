#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>     
#include <sys/time.h>  
#include <unistd.h>    
#include <stdio.h>     

void main()
{
    int fileDescriptor, writeBytes;
    char data[100];

    fileDescriptor = open("myfifo", O_NONBLOCK | O_WRONLY);

    if (fileDescriptor == -1)
    {
        perror("Error while opening FIFO file!");
        _exit(0);
    }

    writeBytes = write(fileDescriptor, "Hello there...", 14);

    if (writeBytes == -1)
        perror("Error while writing to FIFO!");

    close(fileDescriptor);
}
