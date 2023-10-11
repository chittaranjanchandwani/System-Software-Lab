#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdbool.h>

#include "./functions/server-constants.h"
#include"./structures/faculty.h"
#include"./structures/student.h"
#include"./structures/course.h"
#include"./structures/enrollment.h"
#include"./structures/admin.h"
#include"./functions/menu.c"
int client(int socket_fd_client);
bool validateAdmin(struct admin user);
bool validateFaculty(struct faculty user);
bool validateStudent(struct student user);
int main()
{
	int socket_fd_server, socket_fd_client;
	struct sockaddr_in server_addr, client_addr;
	socket_fd_server=socket(AF_INET, SOCK_STREAM, 0);
	socklen_t client_len=sizeof(client_addr);
	if(socket_fd_server<0)
	{
		perror("Error in socket creation");
		return 1;
	}

	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	server_addr.sin_port=htons(8080);

	int bind_status, num;
	bind_status=bind(socket_fd_server,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(bind_status<0)
	{
		perror("bind failure");
		close(socket_fd_server);
		return 1;
	}
	if(listen(socket_fd_server,5)<0)
	{
		perror("failure during listen");
		close(socket_fd_server);
		return 1;
	}
	printf("Listening for Connerctions\n");
	while(1)
	{
		socket_fd_client=accept(socket_fd_server, (struct sockaddr*)&client_addr,&client_len);
		if(socket_fd_client<0)
		{
			perror("Failure in accept");
			continue;
		}

		pid_t childid=fork();
		if(childid==0)
		{
			close(socket_fd_server);
			client(socket_fd_client);
			return 1;
		}
		else if(childid>0)
		{
			close(socket_fd_client);
		}
		else
		{
			perror("Error in fork");
			close(socket_fd_client);
		}
	}
	close(socket_fd_server);
	return 0;
}
int client(int socket_fd_client)
{

	bool res;
	int choice;
	while(1) {			// for login
		recv(socket_fd_client, &choice, sizeof(choice), 0);
		switch(choice) {
			case 1:
				struct admin userAdmin;
				//char curr_loginId[25];
				recv(socket_fd_client, &userAdmin, sizeof(struct faculty), 0);
				//strcpy(curr_loginId,curr_user.login_id);
				res = validateAdmin(userAdmin);
				send(socket_fd_client, &res, sizeof(res), 0);
				break;
			case 2:
				struct faculty userFaculty;
				//char curr_loginId[25];
				recv(socket_fd_client, &userFaculty, sizeof(struct faculty), 0);
				//strcpy(curr_loginId,curr_user.login_id);
				res = validateFaculty(userFaculty);
				send(socket_fd_client, &res, sizeof(res), 0);
				break;
			case 3:
				struct student userStudent;
				recv(socket_fd_client, &userStudent, sizeof(struct student), 0);
				//strcpy(curr_loginId,curr_user.login_id);
				res = validateStudent(userStudent);
				send(socket_fd_client, &res, sizeof(res), 0);
				break;
			default:
		}
	}
	close(socket_fd_client);
	return 1;
}
bool validateAdmin(struct admin user)
{
	int fd=open("/home/chittaranjan-chandwani/SS_Mini_Project/admin.data",O_RDONLY,0744);
	bool result;
	struct admin temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0 ; 	    
	lock.l_len=0;           
	lock.l_pid=getpid();
	fl1=fcntl(fd,F_SETLKW,&lock);
	read(fd,&temp,sizeof(struct admin));
	if(strcmp(temp.password,user.password)==0)
	{
		result=true;
	}	
	else
	{
		result=false;
	}						
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	return result;
}
bool validateFaculty(struct faculty user)
{
	int fd=open("/home/chittaranjan-chandwani/SS_Mini_Project/faculty.data",O_RDONLY,0744);
	bool result;
	struct faculty temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0 ; 	    
	lock.l_len=0;           
	lock.l_pid=getpid();
	fl1=fcntl(fd,F_SETLKW,&lock);
	read(fd,&temp,sizeof(struct faculty));
	if(strcmp(temp.password,user.password)==0)
	{
		result=true;
	}	
	else
	{
		result=false;
	}						
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	return result;
}
bool validateStudent(struct student user)
{
	int fd=open("/home/chittaranjan-chandwani/SS_Mini_Project/student.data",O_RDONLY,0744);
	bool result;
	struct student temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0 ; 	    
	lock.l_len=0;           
	lock.l_pid=getpid();
	fl1=fcntl(fd,F_SETLKW,&lock);
	read(fd,&temp,sizeof(struct student));
	if(strcmp(temp.password,user.password)==0)
	{
		result=true;
	}	
	else
	{
		result=false;
	}						
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	return result;
}

	
