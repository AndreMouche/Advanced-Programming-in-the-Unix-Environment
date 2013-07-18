#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";
#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main() {
  int fd;
  if((fd = creat("file.hole",FILE_MODE)) < 0) {
    printf("create error");
    exit(-1);
  }    
  if(write(fd,buf1,10) != 10) {
     printf("buf1 write error");
     exit(-1);
  }
  // offset now 10
  
  if(lseek(fd,16384,SEEK_SET) == -1) {
      printf("lseek error\n");
      exit(-1);   
  }
  // offset now 16384
  if(write(fd,buf2,10) != 10) {
       printf("buf2 write error\n");
       exit(-1); 
   }
   
   // offset now 16394
   exit(10);
  
}
