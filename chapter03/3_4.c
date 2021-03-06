#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int val;
  if(argc != 2) {
     printf("usage:a.out <descriptor#>");
     exit(-1);
  }

  if((val = fcntl(atoi(argv[1]),F_GETFL,0)) < 0){
      printf("fcntl error for fd %d\n",atoi(argv[1]));
      exit(-1);
  }
  
  switch (val & O_ACCMODE) {
  case O_RDONLY:
       printf("read only ");
       break;
  case O_WRONLY:
       printf("write only ");
       break;
  case O_RDWR:
       printf("read write");
       break;
  default:
       printf("unknow access mode\n");
       exit(-1);  	  
  
  }
  
   if(val & O_APPEND) {
      printf("append");
   }

   if(val & O_NONBLOCK) {
      printf("no blocking ");
   }

#if defined(O_SYNC)
   if(val & O_SYNC) {
     printf(",synchronous writes");
    }

#endif

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC)
  if(val * OFYNC) {
     printf(",synchronous writes");
  }
#endif

   putchar('\n');
   exit(0);
}
