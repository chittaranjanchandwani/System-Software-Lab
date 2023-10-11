#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
int admin_menu()
{
    int choice;
    printf("\tWelcome to ADMIN Menu\n");
    printf("1. Add Student Details\n2. View tudent Details\n3. Add Faculty Details\n4. View Faculty Details\n");
    printf("5. Activate Student\n6. Block Student\n7. Modify Student Details\n8. Modify Faculty Details\n9. Logout and Exit");
    printf("\n\nEnter your choice ");
    scanf("%d",&choice);
    return choice;
}
int student_menu()
{
    int choice;
    printf("\tWelcome to Student Menu\n");
    printf("1. View all courses\n2. Enroll in a new course\n3. Drop a course\n");
    printf("4. View Enrolled courses\n5. Change Password\n6. Logout and Exit");
    printf("\n\nEnter your choice ");
    scanf("%d",&choice);
    return choice;
}
int faculty_menu()
{
    int choice;
    printf("\tWelcome to ADMIN Menu\n");
    printf("1. View Offering Courses\n2. Add new Course\n3. Remove Course\n");
    printf("4. Update Course Details\n5. Change Password\n6. Logout and Exit");
    printf("\n\nEnter your choice ");
    scanf("%d",&choice);
    return choice;
}