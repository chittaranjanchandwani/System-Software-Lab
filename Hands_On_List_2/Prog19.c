#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(){

   // d.mknod system call
   int e = mknod("mknod_fifo",S_IFIFO,0);
   if(e<0) 
     perror("mknod returns");
   
   // e.mkfifo library function
   e = mkfifo("mkfifo_fifo",0744);
   if(e<0)
     perror("mkfifo returns");
      
   return 1; 

}
