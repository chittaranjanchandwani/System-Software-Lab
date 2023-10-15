
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <stdlib.h>
#include "./structures/faculty.h"
#include "./structures/student.h"
#include "./structures/course.h"
#include "./structures/enrollment.h"
#include "./structures/admin.h"
// #include"./functions/menu.c"
// #include"./adminFunctions.c"
int menu(int socketfd);
int admin_menu(int sockfd);
int faculty_menu(int sockfd);
int student_menu(int sockfd);
int menu_choice(int sockfd);
void admin_login(int sockfd);
void faculty_login(int sockfd);
void student_login(int sockfd);
void admin_function(int clientfd, int option);
void faculty_function(int clientfd, int option);
void student_function(int clientfd, int option);
int choice1;
int menu_choice(int);
int main()
{
    int socketfd;
    int connection_Status;
    struct sockaddr_in address;
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
        return 1;
    }
    printf("Client to server connection successfully established!\n");
    menu(socketfd);
    menu_choice(socketfd);
    close(socketfd);
    return 0;
}
int menu(int sockfd)
{

    printf("\tWelcome To Academia, Your one stop Course Management system\nLogin Type\n1.Admin\n2.Faculty\n3.Student\nEnter your choice: ");
    scanf("%d", &choice1);

    switch (choice1)
    {
    case 1:
        admin_login(sockfd);
        break;
    case 2:
        faculty_login(sockfd);
        break;
    case 3:
        student_login(sockfd);
        break;
    default:
        printf("\nInvalid choice \n");
        menu(sockfd);
        break;
    }
    return 0;
}
int admin_menu(int sockfd)
{
    int option;
    printf("\tWelcome to ADMIN Menu\n");
    printf("1. Add Student Details\n2. View Student Details\n3. Add Faculty Details\n4. View Faculty Details\n");
    printf("5. Activate Student\n6. Block Student\n7. Modify Student Details\n8. Modify Faculty Details\n9. Logout and Exit");
    printf("\n\nEnter your choice ");
    scanf("%d", &option);
    admin_function(sockfd, option);
    return option;
}
int student_menu(int sockfd)
{
    int option;
    printf("\tWelcome to Student Menu\n");
    printf("1. View all courses\n2. Enroll in a new course\n3. Drop a course\n");
    printf("4. View Enrolled courses\n5. Change Password\n6. Logout and Exit");
    printf("\n\nEnter your choice ");
    scanf("%d", &option);
    student_function(sockfd, option);
    return option;
}
int faculty_menu(int sockfd)
{
    int option;
    printf("\tWelcome to Faculty Menu\n");
    printf("1. View Offering Courses\n2. Add new Course\n3. Remove Course\n");
    printf("4. Update Course Details\n5. Change Password\n6. Logout and Exit");
    printf("\n\nEnter your choice ");
    scanf("%d", &option);
    faculty_function(sockfd, option);
    return option;
}
int menu_choice(int sockfd)
{
    switch (choice1)
    {
    case 1:
        admin_menu(sockfd);
        break;
    case 2:
        faculty_menu(sockfd);
        break;
    case 3:
        student_menu(sockfd);
        break;
    default:
        printf("Invalid Choice!");
        break;
    }
    return 0;
}
void admin_login(int sockfd)
{
    struct admin user;
    bool result;
    // int choice = 1;
    printf("Login id: ");
    scanf("%s", user.loginid);
    printf("Password: ");
    char *password = getpass("");
    strcpy(user.password, password);

    send(sockfd, &choice1, sizeof(choice1), 0);

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
    // int choice = 2;
    printf("\nLogin id: ");
    scanf("%s", user.loginid);
    printf("Password: ");
    char *password = getpass("");
    strcpy(user.password, password);

    send(sockfd, &choice1, sizeof(choice1), 0);

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
    // int choice = 2;
    printf("Login id: ");
    scanf("%s", user.loginid);
    printf("Password: ");
    char *password = getpass("");
    strcpy(user.password, password);

    send(sockfd, &choice1, sizeof(choice1), 0);

    send(sockfd, &user, sizeof(struct student), 0);

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
void admin_function(int clientfd, int option)
{
    int id;
    switch (option)
    {
    case 1:
        struct student st1;
        bool result;
        printf("\nEnter new student details:\n");
        printf("Enter student name:");
        scanf(" %[^\n]", st1.name);
        printf("Enter Student age:");
        scanf(" %d", &st1.age);
        printf("Enter address of student:");
        scanf(" %[^\n]", st1.address);
        printf("Enter student email:");
        scanf(" %[^\n]", st1.email);
        strcpy(st1.password, "999");
        send(clientfd, &option, sizeof(option), 0);
        send(clientfd, &st1, sizeof(struct student), 0);
        recv(clientfd, &st1.id, sizeof(st1.id), 0);
        recv(clientfd, &result, sizeof(result), 0);
        if (result == true)
        {
            printf("\nStudent Successfully added with student id %d and the default password is 999\n", st1.id);
            printf("\nYour login ID is MT{id number}");
        }
        if (result == false)
        {
            printf("\nStudent adding unsuccessful\n");
        }
        // recv(clientfd, &choice_prev, sizeof(choice_prev),0);
        menu_choice(clientfd);
        break;
    case 2:

        int res;
        send(clientfd, &option, sizeof(int), 0);
        struct student st2;
        int studentID;
        printf("Enter Student ID: ");
        scanf("%d", &studentID);
        send(clientfd, &studentID, sizeof(int), 0);

        res = recv(clientfd, &st2, sizeof(struct student), 0);

        if (res == 0)
        {
            printf("Please re-check the Student ID!\n\n");
        }
        else
        {
            printf("Student ID : %d\n", st2.id);
            printf("Name : %s\n", st2.name);
            printf("Age : %d\n", st2.age);
            printf("Address: %s\n", st2.address);
            printf("Email: %s\n", st2.email);
            printf("Status : %s\n\n", st2.status);
            printf("LoginID=%s", st2.loginid);
            printf("Password: %s", st2.password);
        }
        menu_choice(clientfd);
        break;
    case 3:
        struct faculty fac;
        printf("\nEnter new faculty details:\n");
        printf("Enter faculty name:");
        scanf(" %[^\n]", fac.name);
        printf("Enter Department of faculty:");
        scanf(" %[^\n]", fac.department);
        printf("Enter Designation of faculty:");
        scanf(" %[^\n]", fac.designation);
        printf("Enter address of faculty:");
        scanf(" %[^\n]", fac.address);
        printf("Enter faculty email:");
        scanf(" %[^\n]", fac.email);
        strcpy(fac.password, "000");
        send(clientfd, &option, sizeof(option), 0);
        send(clientfd, &fac, sizeof(struct faculty), 0);
        recv(clientfd, &id, sizeof(id), 0);
        recv(clientfd, &result, sizeof(result), 0);
        if (result == true)
        {
            printf("\nFaculty Successfully added with Faculty id %d with default password 000\n", id);
            printf("\nYour Login Id is FT{facultyid}");
        }
        if (result == false)
        {
            printf("\nFaculty adding unsuccessful\n");
        }
        // recv(clientfd, &choice_prev, sizeof(choice_prev),0);
        menu_choice(clientfd);
        break;
    case 4:

        send(clientfd, &option, sizeof(int), 0);
        struct faculty fac2;
        int facultyID;
        printf("Enter Faculty ID: ");
        scanf("%d", &facultyID);
        send(clientfd, &facultyID, sizeof(int), 0);

        res = recv(clientfd, &fac2, sizeof(struct faculty), 0);

        if (res == 0)
        {
            printf("Please re-check the Faculty ID!\n\n");
        }
        else
        {
            printf("Faculty ID : %d\n", fac2.id);
            printf("Name : %s\n", fac2.name);
            printf("Faculty Department : %s\n", fac2.department);
            printf("Faculty Designation: %s\n", fac2.designation);
            printf("Login ID: %s\n\n", fac2.loginid);
            printf("Password: %s\n", fac2.password);
        }
        menu_choice(clientfd);
        break;
    case 5:
        send(clientfd, &option, sizeof(int), 0);

        struct student st3;
        printf("Enter the Student User ID to be activated : ");
        scanf("%d", &st3.id);

        write(clientfd, &st3, sizeof(struct student));

        read(clientfd, &result, sizeof(result));
        if (!result)
        {
            printf("Error activating student,please re-check the User ID!\n\n");
        }
        else
        {
            printf("Succesfully activated the student!\n\n");
        }

        menu_choice(clientfd);
        break;
    case 6:
        send(clientfd, &option, sizeof(int), 0);

        struct student st4;
        printf("Enter the Student User ID to be blocked : ");
        scanf("%d", &st4.id);

        send(clientfd, &st4, sizeof(struct student), 0);

        recv(clientfd, &result, sizeof(result), 0);
        if (!result)
        {
            printf("Error blocking student,please re-check the User ID!\n\n");
        }
        else
        {
            printf("Succesfully blocked the student!\n\n");
        }

        menu_choice(clientfd);
        break;
    case 7:
        send(clientfd, &option, sizeof(int), 0);

        struct student st5;
        printf("Enter the Student User ID to be modified : ");
        scanf("%d", &st5.id);

        printf("New Name of the Student : ");
        scanf(" %[^\n]", st5.name);
        printf("New Age: ");
        scanf("%d", &st5.age);
        getchar();
        printf("New Address: ");
        scanf("%[^\n]", st5.address);
        getchar();
        printf("New email: ");
        scanf("%[^\n]", st5.email);

        send(clientfd, &st5, sizeof(struct student), 0);

        recv(clientfd, &result, sizeof(result), 0);

        if (!result)
        {
            printf("Error modifying the student details,please re-check the User ID!\n\n");
        }
        else
        {
            printf("Succesfully modified the student details!\n\n");
        }
        menu_choice(clientfd);
        break;
    case 8:
        write(clientfd, &option, sizeof(int));

        struct faculty fac3;
        printf("Enter the Faculty ID to be modified : ");
        scanf("%d", &fac3.id);

        printf("New Name of the Faculty : ");
        scanf(" %[^\n]", fac3.name);
        printf("New Department: ");
        scanf("%[^\n]", fac3.department);
        printf("New Designation: ");
        scanf("%[^\n]", fac3.designation);
        printf("New Address: ");
        scanf("%[^\n]", fac3.address);
        printf("New email: ");
        scanf("%[^\n]", fac3.email);

        send(clientfd, &fac3, sizeof(struct faculty), 0);

        recv(clientfd, &result, sizeof(result), 0);

        if (!result)
        {
            printf("Error modifying the student details,please re-check the User ID!\n\n");
        }
        else
        {
            printf("Succesfully modified the student details!\n\n");
        }
        menu_choice(clientfd);
        break;
    case 9:
        send(clientfd, &option, sizeof(int), 0);
        printf("\nThank You For using Academia!\n");
        exit(0);
        break;
    default:
        printf("Invaid Choice!\n");
        menu_choice(clientfd);
        break;
    }
}
void faculty_function(int clientfd, int option)
{
    int id;
    switch (option)
    {
    case 1:
        int count;
        // int len;
        // bool result;
        send(clientfd, &option, sizeof(int), 0);

        struct course c2;
        char facultyid[10];
        getchar();
        printf("Enter your faculty login ID: ");
        scanf("%[^\n]", facultyid);
        getchar();
        printf("Entered Faculty ID : %s\n\n", facultyid);

        send(clientfd, facultyid, sizeof(facultyid), 0);

        recv(clientfd, &count, sizeof(int), 0);

        printf("You are currently offering %d courses.\n", count);
        for (int i = 0; i < count; i++)
        {
            recv(clientfd, &c2, sizeof(struct course), 0);
            printf("\nCourse ID: %d", c2.id);
            printf("\nCourse Name: %s", c2.name);
            // printf("\n");
        }
        printf("\nHELLO");
        menu_choice(clientfd);
        break;
    case 2:
        bool result;

        char rdBuff[1000];
        bzero(rdBuff, sizeof(rdBuff));

        send(clientfd, &option, sizeof(int), 0);
        struct course c1;
        getchar();
        printf("Enter name of the course");
        scanf("%[^\n]", c1.name);
        getchar();
        printf("Enter your faculty login ID : ");
        scanf("%[^\n]", c1.facultyloginid);
        getchar();
        printf("Enter the department :");
        scanf("%[^\n]", c1.department);
        getchar();
        int seats, creds;
        printf("Enter number of seats : ");
        scanf("%d", &seats);
        getchar();
        printf("Enter no.of credits:");
        scanf("%d", &creds);
        // getchar();
        c1.credits = creds;
        c1.no_of_seats = seats;
        c1.no_of_available_seats = seats;
        printf("Hello");
        send(clientfd, &c1, sizeof(struct course), 0);
        recv(clientfd, rdBuff, sizeof(rdBuff), 0);
        recv(clientfd, &result, sizeof(result), 0);
        strcpy(c1.status, "ACTIVE");

        if (!result)
        {
            printf("Error in adding course,please re-check if you entered details correctly!\n\n");
        }
        else
        {
            printf("Succesfully added the course!\n\n");
        }
        printf("%s\n", rdBuff);
        menu_choice(clientfd);
        break;
    case 3:
        send(clientfd, &option, sizeof(option), 0);
        int courseID;
        printf("Enter the course ID to delete: ");
        scanf("%d", &courseID);
        printf("Entered Course ID : %d\n\n", courseID);

        send(clientfd, &courseID, sizeof(int),0);
        recv(clientfd, &result,sizeof(result),0);
        if(result==true)
        {
            printf("Course deleted successfully.\n\n");
        }
        else
        {
            printf("Unable to remove Course:\n\n");
        }
        

        menu_choice(clientfd);
        break;
    case 4:
        break;
    case 5:
        struct faculty fac1;
        send(clientfd, &option, sizeof(int), 0);
        printf("Enter the Faculty login ID : ");
        scanf("%s", fac1.loginid);
        printf("\nEnter new Password(max 10 characters) : ");
        char *pass = getpass("");
        strcpy(fac1.password, pass);

        send(clientfd, &fac1, sizeof(struct faculty), 0);
        recv(clientfd, &result, sizeof(result), 0);
        if (!result)
        {
            printf("Error changing the faculty password,please re-check the login ID!\n\n");
        }
        else
        {
            printf("\nSuccessfully changed the password!\n\n");
        }
        menu_choice(clientfd);
        break;
    case 6:
        break;
    default:
        printf("Invalid choice!\n");
        menu_choice(clientfd);
        break;
    }
}
void student_function(int clientfd, int option)
{
    bool result;
    switch (option)
    {
    case 1:
        send(clientfd, &option, sizeof(int), 0);
        int count;
        struct course c1;
        recv(clientfd, &count, sizeof(int), 0);
        printf("\nTotal available courses are %d.\n", count);
        for (int i = 0; i < count; i++)
        {
            recv(clientfd, &c1, sizeof(struct course), 0);
            printf("\nCourse ID: %d", c1.id);
            printf("\nCourse Name: %s", c1.name);
            printf("\nAvailable Seats: %d", c1.no_of_available_seats);
            printf("\n");
        }
        printf("\n");
        menu_choice(clientfd);
        break;
    case 2:
        int available_seats;
        send(clientfd, &option, sizeof(int), 0);
        struct enrollment enroll;
        printf("Enter your student ID : ");
        scanf("%d", &enroll.stid);
        printf("Enter Course ID to enroll : ");
        scanf("%d", &enroll.cid);
        strcpy(enroll.status, "ACTIVE");

        send(clientfd, &enroll, sizeof(struct enrollment), 0);
        recv(clientfd, &available_seats, sizeof(int), 0);
        printf("\nNumber of available seats: %d\n", available_seats);

        recv(clientfd, &result, sizeof(result), 0);
        if (result == true)
        {
            printf("Successfully enrolled in course!\n\n");
        }
        else
        {
            printf("Unable to enroll!\n\n");
        }

        menu_choice(clientfd);

        break;
    case 3:
        send(clientfd, &option, sizeof(int), 0);
        struct enrollment enroll1;
        printf("Enter your student ID : ");
        scanf("%d", &enroll1.stid);
        printf("Enter Course ID of course you wish to drop : ");
        scanf("%d", &enroll1.cid);
        send(clientfd, &enroll1, sizeof(struct enrollment), 0);
        recv(clientfd, &result, sizeof(result), 0);
        if (result == true)
        {
            printf("Successfully dropped the course!\n\n");
        }
        else
        {
            printf("Unable to drop!\n\n");
        }

        menu_choice(clientfd);
        break;
    case 4:
        count = 0;
        send(clientfd, &option, sizeof(int), 0);
        struct enrollment enroll2;
        int studentID;
        printf("Enter your Student ID: ");
        scanf("%d", &studentID);
        send(clientfd, &studentID, sizeof(int), 0);
        recv(clientfd, &count, sizeof(int), 0);
        printf("\nYou are currently enrolled in %d courses.\n", count);
        for (int i = 0; i < count; i++)
        {
            recv(clientfd, &enroll2, sizeof(struct enrollment), 0);
            printf("\nCourse ID: %d", enroll2.cid);
            // printf("\nCurse name: %s",enroll2.name);
            printf("\n");
        }
        printf("\n");
        menu_choice(clientfd);
        break;
    case 5:

        send(clientfd, &option, sizeof(int), 0);
        struct student st1;
        printf("Enter the Student User ID to change password : ");
        scanf("%s", st1.loginid);

        printf("New Password(max 10 characters) : ");
        char *pass = getpass("");
        strcpy(st1.password, pass);

        send(clientfd, &st1, sizeof(struct student), 0);
        recv(clientfd, &result, sizeof(result), 0);
        if (!result)
        {
            printf("Error changing the password,please re-check the User ID!\n\n");
        }
        else
        {
            printf("\nSuccessfully changed the password!\n\n");
        }
        menu_choice(clientfd);
        return;
        break;
    case 6:
        break;
    default:
        printf("Invalid choice!\n");
        menu_choice(clientfd);
        break;
    }
}