#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
 int v = symlink("dest","dest_symlink");
if(v<0)
{ perror("Failed");return 1;}

int f=link("dest", "dest_hardlink");
if(f<0)
{ perror("Failed"); return 1;}
int e=mknod("dest_FIFO", S_IFIFO,0);

if(e<0)
{perror("Failed");}
return 0;
}
