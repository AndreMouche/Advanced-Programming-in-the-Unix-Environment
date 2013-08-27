#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
int main(void) {
  struct stat statbuf;
  /* turn on set-group-ID and turn off group-execute*/
  if(stat("foo",&statbuf) < 0) {
     printf("stat error for foo");
     exit(-1);
  } 
  
  if(chmod("foo",(statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
     printf("change error for foo\n");
     exit(-1);
  }

  /* set absolute mode to 'rw-r--r--'*/
  if(chmod("bar",S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
     printf("change error for bar");
     exit(-1);
  }
  exit(0);
}
