#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc , char* argv[]) {
    if (argc!=2){
        printf("You are useless");
        return 0;
    }
    int fd;
    char data1[] = "ABCDEFGHIJ";
    char data2[] = "klmnopqrst";
    fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }
    ssize_t bytesWritten = write(fd, data1, sizeof(data1) - 1);
    if (bytesWritten == -1) {
        perror("Error writing data1");
        close(fd);
        return 1;
    }
    off_t newOffset = lseek(fd, 10, SEEK_CUR);
    if (newOffset == -1) {
        perror("Error seeking file pointer");
        close(fd);
        return 1;
    }
    bytesWritten = write(fd, data2, sizeof(data2) - 1);
    if (bytesWritten == -1) {
        perror("Error writing data2");
        close(fd);
        return 1;
    }
    close(fd);
    printf("Data written and file pointer moved successfully.\n");
    return 0;
}
