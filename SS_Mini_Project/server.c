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
#include "./functions/server-constants.h"
#include "./structures/faculty.h"
#include "./structures/student.h"
#include "./structures/course.h"
#include "./structures/enrollment.h"
#include "./structures/admin.h"
// #include"./functions/menu.c"
int client(int socket_fd_client);
bool validateAdmin(struct admin user);
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
	printf("Listening for Connerctions\n");
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
	// int choice,option;
	while (1)
	{ // for login
		recv(socket_fd_client, &choice, sizeof(choice), 0);
		switch (choice)
		{
		case 1:
			struct admin userAdmin;
			struct student st;
			struct faculty fac;
			// char curr_loginId[25];
			recv(socket_fd_client, &userAdmin, sizeof(struct admin), 0);
			// strcpy(curr_loginId,curr_user.login_id);
			res = validateAdmin(userAdmin);
			send(socket_fd_client, &res, sizeof(res), 0);
			break;
		case 2:
			struct faculty userFaculty;
			// char curr_loginId[25];
			recv(socket_fd_client, &userFaculty, sizeof(struct faculty), 0);
			// strcpy(curr_loginId,curr_user.login_id);
			res = validateFaculty(userFaculty);
			send(socket_fd_client, &res, sizeof(res), 0);
			break;
		case 3:
			struct student userStudent;
			recv(socket_fd_client, &userStudent, sizeof(struct student), 0);
			// strcpy(curr_loginId,curr_user.login_id);
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
		// printf("%d", option);
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
				// printf("%d %s",c1.id,c1.name);
				res = addNewCourse(c1, socket_fd_client);
				send(socket_fd_client, &res, sizeof(res), 0);
				break;
			case 3:
				int courseid;
				recv(socket_fd_client,&courseid,sizeof(int),0);
				res=deleteCourse( courseid, socket_fd_client);
				break;
			case 4:
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
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/admin.data", O_RDONLY, 0744);
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
	// close(fd);
	return result;
}
bool validateFaculty(struct faculty user)
{
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/faculty.data", O_RDONLY, 0744);
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
	// printf("val fac: %d",fl1);
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
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/student.data", O_RDONLY, 0744);
	printf("fd: %d",fd);
	bool result = false;
	struct student temp;
	int status;
	char n = user.loginid[2];
	int i = n - '0';
	if(i<0)
	{
		i=1;
	}
	printf("\ni=%d\n",i);
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
	if ((strcmp(temp.loginid, user.loginid) == 0) && (strcmp(temp.password, user.password) == 0))
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
bool addStudent(int socket_fd_client, struct student st)
{
	printf("Welcome to addstudent");
	bool result;
	struct student prev_st;
	int status;
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/student.data", O_RDWR, 0744);
	int flag = 0;
	if (fd == -1 && errno == ENOENT)
	{
		fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/student.data", O_RDWR | O_CREAT | O_APPEND, 0744);
		/*if (fd == -1)
		{
			perror("Error opening database!");
			exit(EXIT_FAILURE);
		}*/
		st.id = 1;
		// flag = 1;
	}
	else if (fd == -1)
	{
		perror("error in opening database");
		exit(EXIT_FAILURE);
	}
	else
	{
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_END;
		lock.l_start = (-1) * sizeof(struct student);
		lock.l_len = sizeof(struct student);
		lock.l_pid = getpid();

		status = fcntl(fd, F_SETLKW, &lock);
		if (status == -1)
		{
			perror("Error in locking");
			exit(EXIT_FAILURE);
		}
		lseek(fd, (-1) * sizeof(struct student), SEEK_END);
		ssize_t fdread = read(fd, &prev_st, sizeof(struct student));
		if (fdread == -1)
		{
			perror("Error in reading");
			exit(EXIT_FAILURE);
		}
		lock.l_type = F_UNLCK;
		status = fcntl(fd, F_SETLK, &lock);
		if (status == -1)
		{
			perror("Error while unlocking student record");
			exit(EXIT_FAILURE);
		}
		st.id = prev_st.id + 1;
	}
	// printf("ID = %d\n", st.id);
	// sprintf(writebuffer, "%s%d\n","Your student ID is ",st.id);
	strcpy(st.status, "ACTIVE");
	sprintf(st.loginid, "MT%d", st.id);
	send(socket_fd_client, &st.id, sizeof(st.id), 0);
	printf("Generated ID: %d\n", st.id);
	ssize_t fdwrite = write(fd, &st, sizeof(struct student));
	if (fdwrite == -1)
	{
		perror("Error in writing");
		result = false;
		exit(EXIT_FAILURE);
	}
	else
	{
		result = true;
	}
	close(fd);
	return result;
}
bool addFaculty(int socket_fd_client, struct faculty fac)
{

	bool result;
	struct faculty prev_fac;
	int status;
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/faculty.data", O_RDWR, 0744);
	int flag = 0;
	if (fd == -1 && errno == ENOENT)
	{
		fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/faculty.data", O_RDWR | O_CREAT | O_APPEND, 0744);
		if (fd == -1)
		{
			perror("Error opening database!");
			exit(EXIT_FAILURE);
		}
		fac.id = 1;
		// flag = 1;
	}
	else if (fd == -1)
	{
		perror("error in opening database");
		exit(EXIT_FAILURE);
	}
	else
	{
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_END;
		lock.l_start = (-1) * sizeof(struct faculty);
		lock.l_len = sizeof(struct faculty);
		lock.l_pid = getpid();

		status = fcntl(fd, F_SETLKW, &lock);
		if (status == -1)
		{
			perror("Error in locking");
			exit(EXIT_FAILURE);
		}
		lseek(fd, (-1) * sizeof(struct faculty), SEEK_END);
		ssize_t fdread = read(fd, &prev_fac, sizeof(struct faculty));
		if (fdread == -1)
		{
			perror("Error in reading");
			exit(EXIT_FAILURE);
		}
		lock.l_type = F_UNLCK;
		status = fcntl(fd, F_SETLK, &lock);
		if (status == -1)
		{
			perror("Error while unlocking student record");
			exit(EXIT_FAILURE);
		}
		fac.id = prev_fac.id + 1;
	}
	sprintf(fac.loginid, "FT%d", fac.id);
	int id = fac.id;
	send(socket_fd_client, &id, sizeof(id), 0);
	printf("Generated ID: %d\n", fac.id);
	ssize_t fdwrite = write(fd, &fac, sizeof(struct faculty));
	if (fdwrite == -1)
	{
		perror("Error in writing");
		result = false;
		exit(EXIT_FAILURE);
	}
	else
	{
		result = true;
	}
	close(fd);
	return result;
}
struct student searchStudentRecord(int ID)
{
	int i = ID;
	struct student st;
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/student.data", O_RDONLY, 0744);

	int status;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i - 1) * sizeof(struct student), SEEK_SET);
	read(fd, &st, sizeof(struct student));

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return st;
}
struct faculty searchFacultyRecord(int ID)
{
	int i = ID;
	struct faculty fac;
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/faculty.data", O_RDONLY, 0744);

	int status;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct faculty);
	lock.l_len = sizeof(struct faculty);
	lock.l_pid = getpid();

	status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i - 1) * sizeof(struct faculty), SEEK_SET);
	read(fd, &fac, sizeof(struct faculty));

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return fac;
}
bool activateStudent(struct student st)
{
	int i = st.id;
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/student.data", O_RDWR, 0744);
	bool result = false;

	int status;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		exit(EXIT_FAILURE);
	}

	struct student user;
	lseek(fd, (i - 1) * sizeof(struct student), SEEK_SET);
	read(fd, &user, sizeof(struct student));

	strcpy(user.status, "ACTIVE");

	lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
	int writeBytes = write(fd, &user, sizeof(struct student));
	if (writeBytes != 0)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	return result;
}
bool blockStudent(struct student st)
{
	int i = st.id;
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/student.data", O_RDWR, 0744);
	bool result = false;

	int status;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		exit(EXIT_FAILURE);
	}

	struct student user;
	lseek(fd, (i - 1) * sizeof(struct student), SEEK_SET);
	read(fd, &user, sizeof(struct student));

	strcpy(user.status, "BLOCKED");

	lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
	int writeBytes = write(fd, &user, sizeof(struct student));
	if (writeBytes != 0)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	return result;
}
bool updateStudent(struct student st)
{
	int i = st.id;
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/student.data", O_RDWR, 0744);
	bool result = false;

	int status;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i - 1) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		exit(EXIT_FAILURE);
	}

	struct student user;
	lseek(fd, (i - 1) * sizeof(struct student), SEEK_SET);
	read(fd, &user, sizeof(struct student));

	strcpy(user.name, st.name);
	user.age = st.age;
	strcpy(user.address, st.address);
	strcpy(user.email, st.email);
	lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
	int writeBytes = write(fd, &user, sizeof(struct student));
	if (writeBytes != 0)
	{
		result = true;
	}

	else
	{
		result = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	return result;
}
bool updateFaculty(struct faculty fac)
{
	int i = fac.id;
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/faculty.data", O_RDWR, 0744);
	bool result = false;

	int status;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct faculty);
	lock.l_len = sizeof(struct faculty);
	lock.l_pid = getpid();

	status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		exit(EXIT_FAILURE);
	}

	struct faculty user;
	lseek(fd, (i - 1) * sizeof(struct faculty), SEEK_SET);
	read(fd, &user, sizeof(struct faculty));

	strcpy(user.name, fac.name);
	strcpy(user.department, fac.department);
	strcpy(user.designation, fac.designation);

	strcpy(user.address, fac.address);
	strcpy(user.email, fac.email);
	lseek(fd, (-1) * sizeof(struct faculty), SEEK_CUR);
	int writeBytes = write(fd, &user, sizeof(struct faculty));
	if (writeBytes != 0)
	{
		result = true;
	}

	else
	{
		result = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	return result;
}
bool addNewCourse(struct course record, int sd)
{

	bool result;
	int status;
	char wrBuff[1000];
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/course.data", O_RDWR, 0744);

	//printf("\n fd: %d", fd);
	if (fd == -1 && errno == ENOENT)
	{
		fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/course.data", O_RDWR | O_CREAT | O_APPEND, 0744);
		/*if (fd == -1)
		{
			perror("Error opening database!");
			exit(EXIT_FAILURE);
		}*/
		record.id = 1;
		// flag = 1;
	}
	else if (fd == -1)
	{
		perror("error in opening database");
		exit(EXIT_FAILURE);
	}
	else
	{
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_END;
		lock.l_start = (-1) * sizeof(struct course);
		lock.l_len = sizeof(struct course);
		lock.l_pid = getpid();

		status = fcntl(fd, F_SETLKW, &lock);
		// printf("\nstatus=%d",status);
		struct course prev_Course;
		if (status == -1)
		{
			perror("Error while locking");
			exit(EXIT_FAILURE);
		}

		lseek(fd, (-1) * sizeof(struct course), SEEK_END);

		read(fd, &prev_Course, sizeof(struct course));
		record.id = prev_Course.id + 1;
		lock.l_type = F_UNLCK;
		status = fcntl(fd, F_SETLK, &lock);
		if (status == -1)
		{
			perror("Error while unlocking");
			exit(EXIT_FAILURE);
		}
	}

	printf("Course ID = %d\n", record.id);
	sprintf(wrBuff, "%s%d\n", "Course ID of your new Course is ", record.id);
	int writeBytes = write(fd, &record, sizeof(struct course));
	if (writeBytes == -1)
	{
		result = false;
	}
	else
	{
		result = true;
	}

	close(fd);
	send(sd, wrBuff, sizeof(wrBuff), 0);

	return result;
}
void viewOfferedCourses(char facultyID[10], int sd)
{
	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/course.data", O_RDONLY, 0744);
	struct course c3;
	ssize_t bytesRead;
	char fID[10];
	strcpy(fID, facultyID);
	// reading one by one record
	int count = 0;
	printf("Faculty ID to send courses : %s\n:", fID);
	// count number of course offered by faculty
	while ((bytesRead = read(fd, &c3, sizeof(struct course))) > 0)
	{
		if (strcmp(fID, c3.facultyloginid) == 0)
		{
			count++;
		}
	}

	// send this count to client
	send(sd, &count, sizeof(int), 0);

	lseek(fd, 0, SEEK_SET);

	// sending course details to client
	while ((bytesRead = read(fd, &c3, sizeof(struct course))) > 0)
	{

		if (strcmp(fID, c3.facultyloginid) == 0)
		{
			send(sd, &c3, sizeof(struct course), 0);
		}
	}
	close(fd);
}
bool changeFacultyPassword(struct faculty fac)
{
	char n = fac.loginid[2];
	int i = n - '0';
	bool result = false;

	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/faculty.data", O_RDWR);
	if (fd == -1)
	{
		perror("Error opening faculty.data for password change");
		return false;
	}

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i - 1) * sizeof(struct faculty);
	lock.l_len = sizeof(struct faculty);
	lock.l_pid = getpid();

	if (fcntl(fd, F_SETLKW, &lock) == -1)
	{
		perror("Error locking faculty.data file for password change");
		close(fd);
		return false;
	}

	struct faculty user;
	lseek(fd, (i - 1) * sizeof(struct faculty), SEEK_SET);
	read(fd, &user, sizeof(struct faculty));

	strcpy(user.password, fac.password);

	lseek(fd, (-1) * sizeof(struct faculty), SEEK_CUR);
	int j = write(fd, &user, sizeof(struct faculty));
	if (j != 0)
		result = true;
	else
		result = false;

	lock.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLK, &lock) == -1)
	{
		perror("Error unlocking Faculty.data after password change");
	}

	close(fd);
	return result;
}
bool changeStudentPassword(struct student st)
{
	char n = st.loginid[2];
	int i = n - '0';
	if(i<0)
	{
		i=1;
	}
	bool result = false;

	int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/student.data", O_RDWR);
	if (fd == -1)
	{
		perror("Error opening Student.data");
		return result;
	}

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i - 1) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	if (fcntl(fd, F_SETLKW, &lock) == -1)
	{
		perror("Error locking Student.data");
		close(fd);
		return result;
	}

	struct student user;

	lseek(fd, (i - 1) * sizeof(struct student), SEEK_SET);
	read(fd, &user, sizeof(struct student));
	strcpy(user.password, st.password);
	lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);

	int writeBytes = write(fd, &user, sizeof(struct student));
	if (writeBytes <= 0)
		result = false;
	else
		result = true;

	lock.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLK, &lock) == -1)
	{
		perror("Error unlocking Student.data");
		result = false;
	}

	close(fd);
	return result;
}
void viewAllCourses(int sd)
{
        int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/course.data", O_RDWR | O_CREAT, 0744);
        struct course c;
        ssize_t bytesRead;
        int count = 0;

        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
		lock.l_pid = getpid();

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking Course.data");
                close(fd);
                return;
        }

        // count number of course offered by faculty
        while ((bytesRead = read(fd, &c, sizeof(struct course))) > 0)
        {
                count++;
        }

        // send this count to client
        send(sd, &count, sizeof(int),0);
        // reset pointer back to start
        if (lseek(fd, 0, SEEK_SET) == -1)
        {
                perror("Error seeking in Course.data");
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
                return;
        }

        // send course details to client
        while ((bytesRead = read(fd, &c, sizeof(struct course))) > 0)
        {
                send(sd, &c, sizeof(struct course),0);
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
}
bool enrollStudent(struct enrollment record, int sd)
{
        bool result = false;

        // checking if seats are available
        int courseid = record.cid;
        int seats = availableSeats(courseid);

        send(sd, &seats, sizeof(int),0);
        // if seats available add entry
        if (seats > 0)
        {
                int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/enroll.data", O_RDWR | O_CREAT | O_APPEND, 0744);
                if (fd == -1)
                {
                        perror("Error opening Enroll.data");
                        return false;
                }

                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;
				lock.l_pid = getpid();

                if (fcntl(fd, F_SETLKW, &lock) == -1)
                {
                        perror("Error locking Enroll.data");
                        close(fd);
                        return false;
                }

                int j = write(fd, &record, sizeof(struct enrollment));

                reduceAvailableSeats(courseid);

                lock.l_type = F_UNLCK;
                if (fcntl(fd, F_SETLK, &lock) == -1)
                {
                        perror("Error unlocking Enroll.data");
                }

                close(fd);
                result = true;
        }
        return result;
}
int availableSeats(int cid)
{
        struct course c1;
        ssize_t bytesRead;

        int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/course.data", O_RDONLY, 0744);
        if (fd == -1)
        {
                perror("Error opening file");
                return -1;
        }

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
		lock.l_pid = getpid();

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking file");
                close(fd);
                return -1;
        }

        // reading one by one record
        while ((bytesRead = read(fd, &c1, sizeof(struct course))) > 0)
        {
                if (c1.id == cid)
                {
                        return c1.no_of_available_seats;
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking file");
        }

        close(fd);
        return -1;
}
void reduceAvailableSeats(int cid)
{
        ssize_t bytesRead;
        struct course currCourse;

        int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/course.data", O_RDWR, 0744);
        if (fd == -1)
        {
                perror("Error opening file");
                return;
        }

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking file");
                close(fd);
                return;
        }

        while ((bytesRead = read(fd, &currCourse, sizeof(struct course))) > 0)
        {
                if (currCourse.id == cid)
                {
                        currCourse.no_of_available_seats = currCourse.no_of_available_seats - 1;
                        lseek(fd, -sizeof(struct course), SEEK_CUR);
                        write(fd, &currCourse, sizeof(struct course));
                        break;
                }
        }
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking file");
        }

        close(fd);
}
void dropCourse(struct enrollment enroll, int sd)
{
        struct enrollment buffer;
        ssize_t bytesRead;

        int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/enroll.data", O_RDWR, 0744);
        if (fd == -1)
        {
                perror("Error opening Enroll.data");
                return;
        }
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking Enroll.data for write");
                close(fd);
                return;
        }

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.stid == enroll.stid && buffer.cid == enroll.cid)
                {
                        strcpy(buffer.status,"BLOCKED");
						lseek(fd, -sizeof(struct enrollment), SEEK_CUR);
                        write(fd, &buffer, sizeof(struct enrollment));
						break;
				}		
						
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking Enroll.data");
        }

        bool res=increaseAvailableSeats(enroll.cid);
		send(sd,&res,sizeof(res),0);
        close(fd);

}
bool increaseAvailableSeats(int courseid)
{
        ssize_t bytesRead;
        struct course c;
		bool result;
        int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/course.data", O_RDWR, 0744);
        if (fd == -1)
        {
                perror("Error opening file");
                return false;
        }

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking file");
                close(fd);
                return false;
        }

        while ((bytesRead = read(fd, &c, sizeof(struct course))) > 0)
        {
                if (c.id == courseid)
                {
                        c.no_of_available_seats = c.no_of_available_seats + 1;
                        lseek(fd, -sizeof(struct course), SEEK_CUR);
                        write(fd, &c, sizeof(struct course));
						result=true;
                        break;
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking file");
				result=false;
        }
		
        close(fd);
		return result;
}
void viewEnrolledCourses(int studentID, int sd)
{
        struct enrollment buffer;
        ssize_t bytesRead;
        int sID = studentID;
        int count = 0;

        int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/enroll.data", O_RDWR | O_CREAT, 0744);
        if (fd == -1)
        {
                perror("Error opening Enroll.data");
                return;
        }

        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking Enroll.data for read");
                close(fd);
                return;
        }

        // count number of course offered by faculty
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.stid == sID && strcmp(buffer.status,"ACTIVE")==0)
                 {
					 count++;
				 }      
        }

        // send this count to client
        send(sd, &count, sizeof(int),0);

        // reset file pointer to start of file
        lseek(fd, 0, SEEK_SET);

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.stid == sID)
                {
                        send(sd, &buffer, sizeof(struct enrollment),0);
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking Enroll.data");
        }

        close(fd);
}
/*bool deleteCourse(int courseID, int sd)
{
        int fd = open("/home/chittaranjan-chandwani/SS_Mini_Project/enroll.data", O_RDWR, 0744);
        
        int cID = courseID;
        struct course buffer;
        ssize_t bytesRead;
        bool found = false;

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                if (buffer.courseID != cID)
                {
                        lseek(tmp_fd, 0, SEEK_END);
                        write(tmp_fd, &buffer, sizeof(struct course));
                }
        }
        removeAllEnrollments(cID);
        close(fd);
        close(tmp_fd);
}*/