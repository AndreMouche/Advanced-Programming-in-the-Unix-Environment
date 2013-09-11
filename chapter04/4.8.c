#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(void) {
   if(chdir("/tmp") < 0) {
      printf("chdir failed");
      exit(3);
   }

   printf("chdir to /tmp successed\n");
   exit(0);
}
