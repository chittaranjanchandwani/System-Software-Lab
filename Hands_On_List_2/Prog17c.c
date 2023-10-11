#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    int p = pipe(fd);

    if (p == -1) {
        perror("pipe");
        return 1;
    }

    int ls_pid, wc_pid;

    if ((ls_pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ls_pid == 0) { 
        close(fd[0]); 

        
        int new_stdout = fcntl(fd[1], F_DUPFD, 0); 
        if (new_stdout == -1) {
            perror("fcntl");
            exit(EXIT_FAILURE);
        }

        dup2(new_stdout, STDOUT_FILENO); 
        close(new_stdout);

        execlp("ls", "ls", "-l", NULL);
        perror("execlp"); 
        exit(EXIT_FAILURE);
    } else { 
        close(fd[1]); 

        if ((wc_pid = fork()) == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (wc_pid == 0) { 
            
            int new_stdin = fcntl(fd[0], F_DUPFD, 0); 
            if (new_stdin == -1) {
                perror("fcntl");
                exit(EXIT_FAILURE);
            }

            dup2(new_stdin, STDIN_FILENO); 
            close(new_stdin); 

            execlp("wc", "wc", NULL);
            perror("execlp"); 
            exit(EXIT_FAILURE);
        } else { 
            close(fd[0]); 
            wait(NULL);
            wait(NULL);
        }
    }
    
    return 0;
}
