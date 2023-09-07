//#define _GNU_SOURCE 
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>
#include<stdlib.h>
#include<sched.h>
int main()
{
	pid_t pid;
	pid=getpid();
	struct sched_param parameters ;
        int policy = sched_getscheduler(pid);
	if (policy == -1) 
	{
        	perror("Error getting scheduling policy");
        	return 1;
    	}
	if (sched_getparam(pid, &parameters) == -1)
       	{
        	perror("Error getting scheduling parameters");
       		return 1;
    	}
	printf("The current process has the process ID=%d and the Scheduling policy it follows is=",pid);
	if(policy==SCHED_FIFO)
	{
		printf("First in First out (FIFO)\n");
	}
	else if(policy==SCHED_RR)
	{
		printf("Round Robin (RR)\n");
	}
	else if(policy==SCHED_OTHER)
	{
		printf("SCHED_OTHER\n");
	}
	else
	{
		printf("Unknown\n");
	}
	//sleep(30);
	printf("Menu\n");
	printf("1. First in First Out(FIFO)\n2. Round Robin (RR)\n");
	int choice;
	printf("Enter the Scheduling policy you want to select for the current process=");
	scanf("%d",&choice);
	struct sched_param new_parameters;
	if(choice==1)
	{
		new_parameters.sched_priority=50;
		if(sched_setscheduler(pid, SCHED_FIFO, &new_parameters) == -1)
	       	{
           		perror("Error changing the scheduling policy");
            		return 1;
		}
		else
		{
			printf("Scheduling Policy changed to FIFO\n");
			return 0;
		}
        }
	else if(choice==2)
	{
		new_parameters.sched_priority=99;
		if(sched_setscheduler(pid,SCHED_RR, &new_parameters)==-1)
		{
			perror("Error changing the scheduling policy\n");
			return 1;
		}
		else
		{
			printf("Scheduling policy changed to RR\n");
		}
	}
	else
	{
		printf("Unvalid Choice Selected!!\n");
	}
	return 0;
}

