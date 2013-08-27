#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
int main( void ) {
   if(open("tmpfile",O_RDWR) < 0) {
       printf("open error!\n");
       exit(-1);
   }
   
   if(unlink("tmpfile") < 0) {
       printf("unlink error\n");
       exit(-1);
   }

   printf("file unlinked\n");
   sleep(15);
   printf("Done\n");
   exit(0);
}
