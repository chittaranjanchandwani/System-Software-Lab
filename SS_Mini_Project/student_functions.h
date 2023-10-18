#ifndef STUDENT
#define STUDENT

void viewAllCourses(int sd);
void reduceAvailableSeats(int cid);
int availableSeats(int cid);
bool increaseAvailableSeats(int courseid);
bool enrollStudent(struct enrollment record, int sd);
void dropCourse(struct enrollment enroll, int sd);
void viewEnrolledCourses(int studentID, int sd);
bool changeStudentPassword(struct student st);

void viewAllCourses(int sd)
{
        int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/course.data", O_RDWR | O_CREAT, 0744);
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

        while ((bytesRead = read(fd, &c, sizeof(struct course))) > 0)
        {
                if (strcmp(c.status, "ACTIVE") == 0)
                {
                        count++;
                }
                else
                {
                        continue;
                }
        }

        // send this count to client
        send(sd, &count, sizeof(int), 0);
        if (count == 0)
        {
                return;
        }

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
                if (strcmp(c.status, "ACTIVE") == 0)
                {
                        send(sd, &c, sizeof(struct course), 0);
                }
                else
                {
                        continue;
                }
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
        ssize_t bytesRead;
        struct enrollment en;
        send(sd, &seats, sizeof(int), 0);
        // if seats available add entry
        if (seats > 0)
        {
                int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/enroll.data", O_RDWR | O_CREAT | O_APPEND, 0744);
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
                while (bytesRead = read(fd, &en, sizeof(struct enrollment)) > 0)
                {
                        if (record.stid == en.stid && record.cid == en.cid && strcmp(en.status, "ACTIVE") == 0)
                        {
                                return false;
                        }
                }
                int j = write(fd, &record, sizeof(struct enrollment));
                if (j <= 0)
                {
                        perror("error in write");
                        exit(EXIT_FAILURE);
                }

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

        int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/course.data", O_RDONLY, 0744);
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

        int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/course.data", O_RDWR, 0744);
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
        lock.l_pid = getpid();

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
                        lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
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
        bool res;
        int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/enroll.data", O_RDWR, 0744);
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
        lock.l_pid = getpid();

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking Enroll.data for write");
                close(fd);
                return;
        }

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.stid == enroll.stid && buffer.cid == enroll.cid && strcmp(buffer.status, "ACTIVE") == 0)
                {
                        strcpy(buffer.status, "BLOCKED");
                        lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
                        write(fd, &buffer, sizeof(struct enrollment));
                        res = true;
                        break;
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking Enroll.data");
        }
        if (res == true)
        {
                res = increaseAvailableSeats(enroll.cid);
        }

        send(sd, &res, sizeof(res), 0);
        close(fd);
}
bool increaseAvailableSeats(int courseid)
{
        ssize_t bytesRead;
        struct course c;
        bool result;
        int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/course.data", O_RDWR, 0744);
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
        lock.l_pid = getpid();

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
                        lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
                        write(fd, &c, sizeof(struct course));
                        result = true;
                        break;
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking file");
                result = false;
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

        int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/enroll.data", O_RDWR | O_CREAT, 0744);
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
        lock.l_pid = getpid();

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking Enroll.data for read");
                close(fd);
                return;
        }

        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.stid == sID && strcmp(buffer.status, "ACTIVE") == 0)
                {
                        count++;
                }
        }

        // send this count to client
        send(sd, &count, sizeof(int), 0);

        // reset file pointer to start of file
        lseek(fd, 0, SEEK_SET);

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.stid == sID && strcmp(buffer.status, "ACTIVE") == 0)
                {
                        send(sd, &buffer, sizeof(struct enrollment), 0);
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking Enroll.data");
        }

        close(fd);
}
bool changeStudentPassword(struct student st)
{
        char n = st.loginid[2];
        int i = n - '0';
        if (i < 0)
        {
                i = 1;
        }
        bool result = false;

        int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/student.data", O_RDWR);
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

#endif