
#include <stdio.h>      
#include <unistd.h>
#include <string.h>  
#include <sys/socket.h> 
#include <netinet/ip.h> 
#include<stdbool.h>

#include"./structures/faculty.h"
#include"./structures/student.h"
#include"./structures/course.h"
#include"./structures/enrollment.h"
#include"./structures/admin.h"
#include"./functions/menu.c"
int menu(int socketfd);
void admin_login(int sockfd);
void faculty_login(int sockfd) ;
void student_login(int sockfd);
//int connection_handler(int connFD);
int main(){
    int socketfd; 
    int connection_Status; 

    struct sockaddr_in address;

    //ssize_t readBytes, writeBytes;
    //char dataFromServer[1024];
    //int choice;
    // Create an endpoint for communicate -> here, create the client side point
    // Create a socket for TCP connection using IPv4
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        perror("Error while creating socket!");
        return 1;
    }

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    connection_Status = connect(socketfd, (struct sockaddr *)&address, sizeof(address));
    if (connection_Status == -1)
    {
        perror("Error while connecting to server");
        _exit(0);
    }
    printf("Client to server connection successfully established!\n");
    menu(socketfd);
    //connection_handler(socketfd);
    close(socketfd);
    return 0;
}
int menu(int sockfd) 
{
    int choice1,choice2;
    printf("\tWelcome To Academia, Your one stop Course Management system\nLogin Type\n1.Admin\n2.Faculty\n3.Student\nEnter your choice: ");
	scanf("%d",&choice1);
  

    switch(choice1) {
        case 1:
            admin_login(sockfd);
            choice2=admin_menu();
            //admin_functions(sockfd);
            printf("Admin");
            break;
        case 2:
            faculty_login(sockfd);
            choice2=faculty_menu();
            //faculty_funtions(sockfd);
            printf("Faculty");
            break;
        case 3:
            student_login(sockfd);
            choice2=student_menu();
            //show_menu(3);
            printf("Student");
            break;
        default:
            printf("\nInvalid choice \n");
            menu(sockfd);
            break;

    }
    return 0;
}
void admin_login(int sockfd) 
{
    struct admin user;
    bool result;
    int choice = 1;
    printf("Login id: ");
    scanf("%s", user.loginid);
    printf("Password: ");
    char* password = getpass("");
    strcpy(user.password, password);

    send(sockfd, &choice, sizeof(choice), 0);

    send(sockfd, &user, sizeof(struct admin), 0);

    recv(sockfd, &result, sizeof(result), 0);
    
    if (result == true) 
    {
        printf("\nLogin Successful \n\n");
    }
    else 
    {
        printf("\nInvalid credentials\n\n");
        menu(sockfd);
    }

    return;
}

void faculty_login(int sockfd)
 {
    struct faculty user;
    bool result;
    int choice = 2;
    printf("\nLogin id: ");
    scanf("%s", user.loginid);
    printf("Password: ");
    char* password = getpass("");
    strcpy(user.password, password);

    send(sockfd, &choice, sizeof(choice), 0);

    send(sockfd, &user, sizeof(struct faculty), 0);

    recv(sockfd, &result, sizeof(result), 0);

    if (result == true) 
    {
        printf("\nLogin Successful \n\n");
    }
    else 
    {
        printf("\nInvalid credentials\n\n");
        menu(sockfd);
    }

    return;
}

void student_login(int sockfd) 
{
    struct student user;
    bool result;
    int choice = 2;
    printf("Login id: ");
    scanf("%s", user.loginid);
    printf("Password: ");
    char* password = getpass("");
    strcpy(user.password, password);

    send(sockfd, &choice, sizeof(choice), 0);

    send(sockfd, &user, sizeof(struct student), 0);

    recv(sockfd, &result, sizeof(result), 0);

    if (result == true) {
        printf("\nLogin Successful \n\n");
    }
    else {
        printf("\nInvalid credentials\n\n");
        menu(sockfd);
    }

    return;
}
/*int connection_handler(int socket_fd)
{
    char inputBuffer[1000], outputBuffer[1000]; // A buffer used for reading from / writting to the server
    ssize_t readBytes, writeBytes;            // Number of bytes read from / written to the socket

    char temp[1000];

    do
    {
        bzero(inputBuffer, sizeof(inputBuffer)); // Empty the read buffer
        bzero(temp, sizeof(temp));
        readBytes = read(socket_fd, inputBuffer, sizeof(inputBuffer));
        if (readBytes == -1)
            perror("Error while reading from client socket!");
        else if (readBytes == 0){
            return 0;
        }
        else if (strchr(inputBuffer, '^') != NULL)
        {
            // Skip read from client
            strncpy(temp, inputBuffer, strlen(inputBuffer) - 1);
            printf("%s\n", temp);
            writeBytes = write(socket_fd, "^", strlen("^"));
            if (writeBytes == -1)
            {
                perror("Error while writing to client socket!");
                break;
            }
        }
        else if (strchr(inputBuffer, '$') != NULL)
        {
            // Server sent an error message and is now closing it's end of the connection
            strncpy(temp, inputBuffer, strlen(inputBuffer) - 2);
            printf("%s\n", temp);
            printf("Closing the connection to the server now!\n");
            break;
        }
        else
        {
            bzero(outputBuffer, sizeof(outputBuffer)); // Empty the write buffer

            if (strchr(inputBuffer, '#') != NULL)
                strcpy(outputBuffer, getpass(inputBuffer));
            else{
               printf("%s", inputBuffer);
               scanf("%[^\n]%*c", outputBuffer); // Take user input!
            }
               writeBytes = write(socket_fd, outputBuffer, strlen(outputBuffer));
               if (writeBytes == -1){
                 perror("Error while writing to client socket!");
                 printf("Closing the connection to the server now!\n");
                 break;
               }
            }
    } while(readBytes > 0);

    close(socket_fd);
}*/
