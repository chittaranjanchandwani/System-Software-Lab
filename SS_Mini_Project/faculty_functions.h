#ifndef FACULTY
#define FACULTY

bool addNewCourse(struct course record, int sd);
void viewOfferedCourses(char facultyID[10], int sd);
bool changeFacultyPassword(struct faculty fac);
bool deleteCourse(int courseID, int sd);
bool updateCourseDetails(struct course modCourse);
bool removeAllEnrollments(int cid);
bool unenrollLastStudents(struct course currCourse);
void viewOfferedCourses(char facultyID[10], int sd)
{
    int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/course.data", O_RDONLY, 0744);
    struct course c3;
    ssize_t bytesRead;
    char fID[10];
    strcpy(fID, facultyID);
    // reading one by one record
    int count = 0;
    // count number of course offered by faculty
    while ((bytesRead = read(fd, &c3, sizeof(struct course))) > 0)
    {
        if (strcmp(fID, c3.facultyloginid) == 0 && strcmp(c3.status, "ACTIVE") == 0)
        {
            count++;
        }
    }
    send(sd, &count, sizeof(int), 0);

    lseek(fd, 0, SEEK_SET);

    // sending course details to client
    while ((bytesRead = read(fd, &c3, sizeof(struct course))) > 0)
    {

        if (strcmp(fID, c3.facultyloginid) == 0 && strcmp(c3.status, "ACTIVE") == 0)
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

    int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/faculty.data", O_RDWR);
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

bool deleteCourse(int courseID, int sd)
{
    int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/course.data", O_RDWR, 0744);

    int cID = courseID;
    struct course buffer;
    ssize_t bytesRead;
    bool result = false;
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
    // sending course details to client
    while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
    {
        if (buffer.id == cID)
        {
            strcpy(buffer.status, "BLOCKED");
            lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
            write(fd, &buffer, sizeof(struct course));
            result = true;
            break;
        }
    }
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("Error unlocking file");
    }
    result = removeAllEnrollments(cID);
    close(fd);
    return result;
}
bool removeAllEnrollments(int cid)
{
    struct enrollment buffer;
    ssize_t bytesRead;
    bool result;
    int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/enroll.data", O_RDWR, 0744);
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
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

    // sending course details to client
    while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
    {
        if (buffer.cid == cid)
        {
            strcpy(buffer.status, "BLOCKED");
            lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
            write(fd, &buffer, sizeof(struct enrollment));
            result = true;
        }
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("Error unlocking Enroll.data");
        result = false;
    }
    close(fd);
    return result;
}
bool updateCourseDetails(struct course modCourse)
{
    int cID = modCourse.id;
    int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/course.data", O_RDWR, 0744);
    if (fd == -1)
    {
        perror("Error opening file for updating");
        return false;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid(); // Lock the whole file

    if (fcntl(fd, F_SETLKW, &lock) == -1)
    {
        perror("Error locking file for updating");
        close(fd);
        return false;
    }

    bool result = false;
    ssize_t bytesRead;
    struct course currCourse;
    while ((bytesRead = read(fd, &currCourse, sizeof(struct course))) > 0)
    {
        if (currCourse.id == cID && strcmp(currCourse.status, "ACTIVE") == 0)
        {
            // int occupied=currCourse.no_of_seats-currCourse.no_of_available_seats;
            // int seats=occupied-modCourse.no_of_seats;
            // if(seats==0)
            // {
            //     currCourse.no_of_available_seats=0;
            //     lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
            //     write(fd, &currCourse, sizeof(struct course));
            //     return true;
            // }
            // else if(seats>0)
            // {
            //     currCourse.no_of_available_seats=0;
            //     lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
            //     write(fd, &currCourse, sizeof(struct course));
            //     result=unenrollLastStudents(modCourse,seats);
            //     return result;
            // }
            // else
            // {
            //     currCourse.no_of_available_seats+=modCourse.no_of_seats-currCourse.no_of_seats;
            //     lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
            //     write(fd, &currCourse, sizeof(struct course));
            //     return true;
            // }
            
            int change_in_seats = currCourse.no_of_seats - modCourse.no_of_seats;
            currCourse.no_of_seats = modCourse.no_of_seats;
            currCourse.no_of_available_seats = currCourse.no_of_available_seats - change_in_seats;
            if (currCourse.no_of_available_seats < 0)
            {
                currCourse.no_of_available_seats = 0;
                result = unenrollLastStudents(modCourse);
            }
            lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
            write(fd, &currCourse, sizeof(struct course));
            //result = true;
            break;
            
        }
    
    }
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("Error unlocking Enroll.data");
        result = false;
    }
    close(fd);
    return result;
}
bool unenrollLastStudents(struct course currCourse)
{
    printf("\n\nUnenrolling students\n\n");
    int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/enroll.data", O_RDWR);
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
    lock.l_pid = getpid(); // Lock the whole file

    if (fcntl(fd, F_SETLKW, &lock) == -1)
    {
        perror("Error locking file for updating");
        close(fd);
        return false;
    }
    lseek(fd,0,SEEK_SET);
    struct enrollment buffer;
    ssize_t bytesRead;
    int seats = currCourse.no_of_seats;
    //printf("Seats=%d\n",seats);
    bool result=true;
    // sending course details to client
    // while(seats>0)
    // {   
    //     lseek(fd,(-1)*sizeof(struct enrollment),SEEK_CUR);
    //     bytesRead = read(fd, &buffer, sizeof(struct enrollment));
    //     if (buffer.cid == currCourse.id && strcmp(buffer.status, "ACTIVE")==0)
    //     {
    //             strcpy(buffer.status, "BLOCKED");
    //             lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
    //             write(fd, &buffer, sizeof(struct enrollment));
    //             lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
    //             result = true;
    //             seats--;
    //     }
    //     else
    //     {
    //         continue;
    //     }
            
    // }
    // lock.l_type = F_UNLCK;
    //     if (fcntl(fd, F_SETLK, &lock) == -1)
    //     {
    //         perror("Error unlocking Enroll.data");
    //         result = false;
    //     }
    //     close(fd);
    //     return result;

    if (seats == 0)
    {
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
            if (buffer.cid == currCourse.id && strcmp(buffer.status, "ACTIVE")==0)
            {
                strcpy(buffer.status, "BLOCKED");
                //printf("Active student id wwhich has been blocked: %d",buffer.stid);
                lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
                write(fd, &buffer, sizeof(struct enrollment));
                result = true;
            }
    
        }
        
    }
    else
    {
        //printf("Entered the else code...\n");
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
            //printf ("buffer.cid : %d, currCourse.id : %d\n", buffer.cid ,currCourse.id);
            if (buffer.cid == currCourse.id && seats > 0 && strcmp(buffer.status, "ACTIVE")==0)
            {
                //printf("Active student id wwhich has  not been blocked: %d",buffer.stid);
                seats--;
                if (seats == 0)
                {
                    break;
                }
            }

        }
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
            if (buffer.cid == currCourse.id && strcmp(buffer.status, "ACTIVE")==0)
            {
                //printf("Student id: %d",buffer.stid);
                strcpy(buffer.status, "BLOCKED");
                lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
                write(fd, &buffer, sizeof(struct enrollment));
                result = true;
            }
        }
      
    
    }
    lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
            perror("Error unlocking Enroll.data");
            result = false;
        }
        close(fd);
        return result;
}
bool addNewCourse(struct course record, int sd)
{

    bool result;
    int status;
    char wrBuff[1000];
    int fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/course.data", O_RDWR, 0744);
    if (fd == -1 && errno == ENOENT)
    {
        fd = open("/home/chittaranjan-chandwani/ACADEMIA_MT2023193/course.data", O_RDWR | O_CREAT | O_APPEND, 0744);
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
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        lock.l_pid = getpid();

        status = fcntl(fd, F_SETLKW, &lock);

        if (status == -1)
        {
            perror("Error while locking");
            exit(EXIT_FAILURE);
        }
        struct course prev_Course;
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
#endif