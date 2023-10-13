#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main(){
    int semid; 
    struct sembuf sb; 
    union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
    } semarg;

    key_t key = ftok(".",'C');
    if(key==-1){
      perror("ftok returns");
      return 1;
    }

    semid = semget(key,1,IPC_CREAT|IPC_EXCL|0666);
    if(semid==-1){
      perror("semget returns");
      return 1;
    }

    // Initialize the semaphore to a count of 5
    semarg.val = 5;
    if(semctl(semid,0,SETVAL,semarg)==-1){
      perror("semctl returns");
      return 1;
    }

    printf("Semaphore created with ID %d and initial count %d\n",semid,semarg.val);
  

    return 1;
}
