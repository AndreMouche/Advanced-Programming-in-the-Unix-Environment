#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXLINE 4096               /* max line length */
 
int main(void) {
  char buf[MAXLINE];
  pid_t pid;
  int status;
   
  printf("%% "); // print prompt
  while(fgets(buf,MAXLINE,stdin) != NULL){
     if(buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = 0; // replace new line with none
     }
     
     if((pid = fork()) < 0) {
        printf("fork error");
        exit(0);
     }
     if(pid == 0) { //child
       execlp(buf,buf,(char *)0);
       printf("couldn't execute:%s\n",buf);
       exit(127);
     }
     
     //parent
     if((pid = waitpid(pid,&status,0) ) < 0) {
        printf("waitpid wrror");
        exit(1);
     }

     printf("%% ");
  }
   exit(0);
  
}
