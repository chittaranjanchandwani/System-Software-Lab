#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
    int pipe_fd[2];
    pid_t child_pid;
    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // Fork a child process
    if ((child_pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (child_pid == 0) { // Child process
        // Redirect stdout to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]); // Close the read end of the pipe
        
        // Execute the "ls -l" command
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        close(pipe_fd[1]); // Close the write end of the pipe
        
        // Redirect stdin to the read end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);
        
        // Execute the "grep ^d | wc" command
        execlp("sh", "sh", "-c", "grep ^d | wc -l", (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
