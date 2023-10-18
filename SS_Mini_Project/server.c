#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <errno.h>
#include "./structures/faculty.h"
#include "./structures/student.h"
#include "./structures/course.h"
#include "./structures/enrollment.h"
#include "./structures/admin.h"
#include "./admin_functions.h"
#include "./student_functions.h"
#include "./faculty_functions.h"
int client(int socket_fd_client);
/*bool validateAdmin(struct admin user);
bool validateFaculty(struct faculty user);
bool validateStudent(struct student user);
bool addStudent(int socket_fd_client, struct student st);
bool addFaculty(int socket_fd_client, struct faculty fac);
struct student searchStudentRecord(int ID);
struct faculty searchFacultyRecord(int ID);
bool activateStudent(struct student st);
bool blockStudent(struct student st);
bool updateFaculty(struct faculty fac);
bool updateStudent(struct student fac);
bool addNewCourse(struct course record, int sd);
void viewOfferedCourses(char facultyID[10], int sd);
bool changeFacultyPassword(struct faculty fac);
bool changeStudentPassword(struct student st);
void viewAllCourses(int sd);
void reduceAvailableSeats(int cid);
int availableSeats(int cid);
bool increaseAvailableSeats(int courseid);
bool enrollStudent(struct enrollment record, int sd);
void dropCourse(struct enrollment enroll, int sd);
void viewEnrolledCourses(int studentID, int sd);
bool deleteCourse(int courseID, int sd);
bool removeAllEnrollments(int cid);
bool unenrollLastStudents(struct course currCourse);
bool updateCourseDetails(struct course modCourse);*/
int main()
{
	int socket_fd_server, socket_fd_client;
	struct sockaddr_in server_addr, client_addr;
	socket_fd_server = socket(AF_INET, SOCK_STREAM, 0);
	socklen_t client_len = sizeof(client_addr);
	if (socket_fd_server < 0)
	{
		perror("Error in socket creation");
		return 1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080);

	int bind_status, num;
	bind_status = bind(socket_fd_server, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (bind_status < 0)
	{
		perror("bind failure");
		close(socket_fd_server);
		return 1;
	}
	if (listen(socket_fd_server, 5) < 0)
	{
		perror("failure during listen");
		close(socket_fd_server);
		return 1;
	}
	printf("Listening for Connections\n");
	while (1)
	{
		socket_fd_client = accept(socket_fd_server, (struct sockaddr *)&client_addr, &client_len);
		if (socket_fd_client < 0)
		{
			perror("Failure in accept");
			continue;
		}

		pid_t childid = fork();
		if (childid == 0)
		{
			close(socket_fd_server);
			client(socket_fd_client);
			return 1;
		}
		else if (childid > 0)
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
	int choice, option;
	bool res;
	while (1)
	{ // for login
		recv(socket_fd_client, &choice, sizeof(choice), 0);
		switch (choice)
		{
		case 1:
			struct admin userAdmin;
			struct student st;
			struct faculty fac;
			recv(socket_fd_client, &userAdmin, sizeof(struct admin), 0);
			res = validateAdmin(userAdmin);
			send(socket_fd_client, &res, sizeof(res), 0);
			break;
		case 2:
			struct faculty userFaculty;
			recv(socket_fd_client, &userFaculty, sizeof(struct faculty), 0);
			res = validateFaculty(userFaculty);
			send(socket_fd_client, &res, sizeof(res), 0);
			break;
		case 3:
			struct student userStudent;
			recv(socket_fd_client, &userStudent, sizeof(struct student), 0);
			res = validateStudent(userStudent);
			send(socket_fd_client, &res, sizeof(res), 0);
			break;
		default:
			break;
		}
		if (res == true)
		{
			break;
		}
	}

	while (1)
	{
		recv(socket_fd_client, &option, sizeof(option), 0);
		switch (choice)
		{
		case 1:
			switch (option)
			{
			case 1:
				struct student st1;
				recv(socket_fd_client, &st1, sizeof(struct student), 0);
				res = addStudent(socket_fd_client, st1);
				send(socket_fd_client, &res, sizeof(res), 0);
				break;
			case 2:
				struct student st2;
				int studentID;
				recv(socket_fd_client, &studentID, sizeof(int), 0);
				st2 = searchStudentRecord(studentID);
				send(socket_fd_client, &st2, sizeof(struct student), 0);
				break;
			case 3:
				struct faculty fac1;
				recv(socket_fd_client, &fac1, sizeof(struct faculty), 0);
				res = addFaculty(socket_fd_client, fac1);
				send(socket_fd_client, &res, sizeof(res), 0);
				break;
			case 4:
				struct faculty fac2;
				int facultyID;
				recv(socket_fd_client, &facultyID, sizeof(int), 0);
				fac2 = searchFacultyRecord(facultyID);
				send(socket_fd_client, &fac2, sizeof(struct faculty), 0);
				break;
			case 5:
				struct student st3;
				read(socket_fd_client, &st3, sizeof(struct student));
				res = activateStudent(st3);
				write(socket_fd_client, &res, sizeof(res));
				break;
			case 6:
				struct student st4;
				read(socket_fd_client, &st4, sizeof(struct student));
				res = blockStudent(st4);
				write(socket_fd_client, &res, sizeof(res));
				break;
			case 7:
				struct student st5;
				recv(socket_fd_client, &st5, sizeof(struct student), 0);
				res = updateStudent(st5);
				send(socket_fd_client, &res, sizeof(res), 0);
				break;
			case 8:
				struct faculty fac3;
				recv(socket_fd_client, &fac3, sizeof(struct faculty), 0);
				res = updateFaculty(fac3);
				send(socket_fd_client, &res, sizeof(res), 0);
				break;
			case 9:
				break;
			default:
				break;
			}
			break;

		case 2:
			switch (option)
			{
			case 1:
				char facultyID[10];
				recv(socket_fd_client, facultyID, sizeof(facultyID), 0);
				viewOfferedCourses(facultyID, socket_fd_client);
				break;
			case 2:
				struct course c1;
				recv(socket_fd_client, &c1, sizeof(struct course), 0);
				res = addNewCourse(c1, socket_fd_client);
				send(socket_fd_client, &res, sizeof(res), 0);
				break;
			case 3:
				int courseid;
				recv(socket_fd_client,&courseid,sizeof(int),0);
				res=deleteCourse( courseid, socket_fd_client);
				send(socket_fd_client,&res,sizeof(res),0);
				break;
			case 4:
				struct course c2;
                recv(socket_fd_client, &c2, sizeof(struct course),0);
                res = updateCourseDetails(c2);
                send(socket_fd_client, &res, sizeof(res),0);
				break;
			case 5:
				struct faculty fac1;
				recv(socket_fd_client, &fac1, sizeof(struct faculty), 0);
				res = changeFacultyPassword(fac1);
				write(socket_fd_client, &res, sizeof(res));
				break;
			case 6:
				break;
			default:
				break;
			}
			break;

		case 3:
			switch (option)
			{
			case 1:
				viewAllCourses(socket_fd_client);
				break;
			case 2:
				struct enrollment enroll;
                recv(socket_fd_client, &enroll, sizeof(struct enrollment),0);
                res = enrollStudent(enroll, socket_fd_client);
                send(socket_fd_client, &res, sizeof(res),0);
				break;
			case 3:
				struct enrollment enroll2;
                recv(socket_fd_client, &enroll2, sizeof(struct enrollment),0);
                dropCourse(enroll2, socket_fd_client);
				break;
			case 4:
				int studentID;
                recv(socket_fd_client, &studentID, sizeof(int),0);
                viewEnrolledCourses(studentID, socket_fd_client);
				break;
			case 5:
				struct student st6;
                recv(socket_fd_client, &st6, sizeof(struct student),0);
                res = changeStudentPassword(st6);
                send(socket_fd_client, &res, sizeof(res),0);
				break;
			case 6:
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		if (option == 9)
		{
			break;
		}
	}
	close(socket_fd_client);
	return 0;
}
bool validateAdmin(struct admin user)
{
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/admin.data", O_RDONLY, 0744);
	bool result;
	struct admin temp;
	int status;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		close(fd);
		exit(EXIT_FAILURE);
	}
	read(fd, &temp, sizeof(struct admin));
	if (strcmp(temp.loginid, user.loginid) == 0 && strcmp(temp.password, user.password) == 0)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	return result;
}
bool validateFaculty(struct faculty user)
{
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/faculty.data", O_RDONLY, 0744);
	bool result;
	struct faculty temp;
	int status;
	char n = user.loginid[2];
	int i = n - '0';
	if(i<0)
	{
		i=1;
	}
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i - 1) * sizeof(struct faculty);
	lock.l_len = sizeof(struct faculty);
	lock.l_pid = getpid();
	status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		close(fd);
		exit(EXIT_FAILURE);
	}
	lseek(fd, (i - 1) * sizeof(struct faculty), SEEK_SET);
	read(fd, &temp, sizeof(struct faculty));

	if (strcmp(temp.loginid, user.loginid) == 0 && strcmp(temp.password, user.password) == 0)
	{
		result = true;
	}
	else
	{
		result = false;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	return result;
}
bool validateStudent(struct student user)
{
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/student.data", O_RDONLY, 0744);
	//printf("fd: %d",fd);
	bool result = false;
	struct student temp;
	int status;
	char n = user.loginid[2];
	int i = n - '0';
	if(i<0)
	{
		i=1;
	}
	//printf("\ni=%d\n",i);
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i - 1) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();
	status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		 close(fd);
		 exit(EXIT_FAILURE);
		//return false;
	}
	lseek(fd, (i - 1) * sizeof(struct student), SEEK_SET);
	read(fd, &temp, sizeof(struct student));
	if ((strcmp(temp.loginid, user.loginid) == 0) && (strcmp(temp.password, user.password) == 0) && (strcmp(temp.status,"ACTIVE")==0))
	{
		result = true;
	}
	else
	{
		result = false;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	return result;
}

