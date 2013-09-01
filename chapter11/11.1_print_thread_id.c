#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

pthread_t ntid;
void printids(const char *s) {
   pid_t pid;
   pthread_t tid;
   pid = getpid();
   tid = pthread_self();
   printf("%s pid %u tid %u (ox%x)\n",s,(unsigned int)pid,
		   (unsigned int)tid,(unsigned int) tid);

}

void *thr_fn(void *arg){
    printids("new thread:");
    return ((void *)0);
}

int main(void) { 
    int err;
    err = pthread_create(&ntid,NULL,thr_fn,NULL);
    if (err != 0) {
       printf("can't create thread:%s\n",strerror(err));
       exit(-1);
    }

    printids("main thread:");
    //there exists competition between main thread and new thread
    //if don't sleep,the program may exit before the new is finished
    sleep(1);
    exit(0);
}

