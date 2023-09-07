#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/resource.h>
int main()
{
	pid_t  pid=getpid();
	int priority_of_process=getpriority(PRIO_PROCESS, pid);
	printf("The Priority of our Program is: %d\n", priority_of_process);
	while(1);
	return 0;
}
