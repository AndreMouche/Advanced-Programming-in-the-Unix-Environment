/**
 *Output:
 thread 1 start
 thread 2 push complete
 thread 2 start
 thread 2 push complete
 thread 1 exit code 1
 cleanup:thread 2 second handler
 cleanup:thread 2 first handler
 thread 2 exit code 2

 * 
 * */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void cleanup(void *arg) {
   printf("cleanup:%s\n",(char *)arg);
}

void * thr_fn1(void *arg) {
   printf("thread 1 start\n");
   pthread_cleanup_push(cleanup,"thread 1 first handler");
   pthread_cleanup_push(cleanup,"thread 1 seconde handler");
   printf("thread 2 push complete\n");
   if(arg) {
      return((void *)1); //cleanup fuction wouldn't run under this condition
   }

   pthread_cleanup_pop(0);
   pthread_cleanup_pop(0);
   return((void *) 1);
}

void * thr_fn2(void *arg) {
   printf("thread 2 start\n");
   pthread_cleanup_push(cleanup,"thread 2 first handler");
   pthread_cleanup_push(cleanup,"thread 2 second handler");
   printf("thread 2 push complete\n");
   if(arg) {
      pthread_exit((void *)2);
   }

   pthread_cleanup_pop(0);
   pthread_cleanup_pop(0);
   pthread_exit((void*)2);
}

int main(void) {
  int err;
  pthread_t tid1,tid2;
  void *tret;
  
  err = pthread_create(&tid1,NULL,thr_fn1,(void *)1);
  if(err != 0) {
     printf("Can't create thread 1:%s\n",strerror(err));
     exit(-1);
  }  

  err = pthread_create(&tid2,NULL,thr_fn2,(void *)1);
  if(err != 0) {
     printf("can't create thread 2:%s\n",strerror(err));
     exit(-1);
  }

  err = pthread_join(tid1,&tret);
  if(err != 0) {
     printf("can't join with thread 1:%s\n",strerror(err));
     exit(-1);
  }

  printf("thread 1 exit code %d\n",(int)tret);

  err = pthread_join(tid2,&tret);
  if(err != 0) {
      printf("can not join with thread 2:%s\n",strerror(err));
      exit(-1);
  }
  printf("thread 2 exit code %d\n",(int)tret);
  return 0;
}
