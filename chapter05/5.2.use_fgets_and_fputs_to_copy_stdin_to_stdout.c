#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 100
int main(void) {
   char buf[MAXLINE];
   while(fgets(buf,MAXLINE,stdin) != NULL) {
      if(fputs(buf,stdout) == EOF) {
	     printf("output error");
		 exit(-1);
	  }
   }

   if(ferror(stdin)) {
       printf("input error");
   }

   exit(0);
}
