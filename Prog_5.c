#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
int main()
{

	const char* filename1="file1.txt";
	const char* filename2="file2.txt";
	const char* filename3="file3.txt";
	const char* filename4="file4.txt";
	const char* filename5="file5.txt";
	int fd1=open(filename1, O_CREAT | O_RDWR, 0744);
	int fd2=open(filename2, O_CREAT | O_RDWR, 0744);
	int fd3=open(filename3, O_CREAT | O_RDWR, 0744);
	int fd4=open(filename4, O_CREAT | O_RDWR, 0744);
	int fd5=open(filename5, O_CREAT | O_RDWR, 0744);
	getchar();
	return 0;
}

