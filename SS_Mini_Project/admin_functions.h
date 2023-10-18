#ifndef ADMIN
#define ADMIN

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

bool addStudent(int socket_fd_client, struct student st)
{
	printf("Welcome to addstudent");
	bool result;
	struct student prev_st;
	int status;
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/student.data", O_RDWR, 0744);
	int flag = 0;
	if (fd == -1 && errno == ENOENT)
	{
		fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/student.data", O_RDWR | O_CREAT | O_APPEND, 0744);
		st.id = 1;
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
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/faculty.data", O_RDWR, 0744);
	int flag = 0;
	if (fd == -1 && errno == ENOENT)
	{
		fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/faculty.data", O_RDWR | O_CREAT | O_APPEND, 0744);
		if (fd == -1)
		{
			perror("Error opening database!");
			exit(EXIT_FAILURE);
		}
		fac.id = 1;
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
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/student.data", O_RDONLY, 0744);

	int status;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i-1) * sizeof(struct student);
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
	if(st.id==i)
	{
		close(fd);
		return st;
	}
	else
	{
		exit(EXIT_FAILURE);
	}
	close(fd);
}
struct faculty searchFacultyRecord(int ID)
{
	int i = ID;
	struct faculty fac;
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/faculty.data", O_RDONLY, 0744);

	int status;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i-1) * sizeof(struct faculty);
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
	if(fac.id==i)
	{
		close(fd);
		return fac;
	}
	else{
		exit(EXIT_FAILURE);
	}

	close(fd);
}
bool activateStudent(struct student st)
{
	int i = st.id;
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/student.data", O_RDWR, 0744);
	bool result = false;

	int status;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i-1) * sizeof(struct student);
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
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}
bool blockStudent(struct student st)
{
	int i = st.id;
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/student.data", O_RDWR, 0744);
	bool result = false;

	int status;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i-1) * sizeof(struct student);
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
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}
bool updateStudent(struct student st)
{
	int i = st.id;
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/student.data", O_RDWR, 0744);
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
	strcpy(user.status,"ACTIVE");
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
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}
bool updateFaculty(struct faculty fac)
{
	int i = fac.id;
	int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/faculty.data", O_RDWR, 0744);
	bool result = false;

	int status;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i-1) * sizeof(struct faculty);
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
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}
#endif