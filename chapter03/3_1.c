#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
int main(void) {
   if(lseek(STDIN_FILENO,0,SEEK_CUR) == -1) {
        printf("can not seek\n");
   } else {
        printf("seek ok\n");
   }
   exit(0);
    
}