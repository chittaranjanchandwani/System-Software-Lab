#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc , char* argv[]) {
    int fd;
    char filename[100];
    char buffer[1]; // Read one character at a time
    ssize_t bytesRead;
    fd = open(argv[1],O_RDONLY); 
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        if (buffer[0] == '\n') {
            write(STDOUT_FILENO, "\n", 1);
        } else {
            write(STDOUT_FILENO, buffer, 1);
        }
    }
    if (bytesRead == -1) {
        perror("Error reading file");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
