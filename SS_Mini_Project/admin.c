#include<fcntl.h>
#include<errno.h>
#include<stdio.h>      
#include<unistd.h>
#include<string.h>  
#include<sys/socket.h> 
#include<netinet/ip.h> 
#include<stdbool.h>
#include"./structures/admin.h"

/*int getNewAdmin(){
	int fd=open("/home/SS_Mini_Project/Admin.data",O_RDONLY,0744);
	if (fd == -1)
    	{
        	return 0;
    	}
	struct admin record;
	lseek(fd,-sizeof(struct admin),SEEK_END);
	read(fd,&record,sizeof(struct admin));
	close(fd);
	return record.id+1;
}*/

int main()
{
	int fd;
    fd=open("/home/chittaranjan-chandwani/SS_Mini_Project/admin.data",O_RDONLY,0744);
	if (fd == -1)
    	{
        	perror("error in opening file");
            return 1;
    	}
	close(fd);
    struct admin first_admin;
	//first_admin.id=getNewAdmin();
	fd=open("/home/chittaranjan-chandwani/SS_Mini_Project/admin.data",O_RDWR | O_CREAT | O_APPEND,0744);
	printf("Enter the name of the admin: ");
	scanf(" %[^\n]",first_admin.name);
	getchar();
	printf("Enter login ID:");
	scanf("%[^\n]",first_admin.loginid);
	getchar();
	printf("Please enter the password(max 10 characters): ");
	scanf(" %[^\n]",first_admin.password);
	write(fd,&first_admin,sizeof(struct admin));
	close(fd);

	return 0;
}