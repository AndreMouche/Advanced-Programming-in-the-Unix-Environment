#include <stdlib.h>
#include <pthread.h>

struct job {
  struct job *j_next;
  struct job *j_prev;
  pthread_t j_id; // tells which thread handles this job
  //more stuff here

};

struct queue {
   struct job *q_head;
   struct job *q_tail;
   pthread_rwlock_t q_lock;
};

//initialize a queue
int queue_init(struct queue *qp) {
   int err;
   qp->q_head = NULL;
   qp->q_tail = NULL;
   err = pthread_rwlock_init(&qp->q_lock,NULL);
   if(err != 0) {
       return (err);
   }

   //continue initialization...
   return (0);
}


//insert a job at the head of the queue
void job_insert(struct queue *qp,struct job *jp) {
   pthread_rwlock_wrlock(&qp->q_lock);
   jp->j_next = qp->q_head;
   jp->j_prev = NULL;
   if(qp->q_head != NULL) {
       qp->q_head->j_prev = jp;
   } else {
       qp->q_tail = jp;  //list was empty
   }

   qp->q_head = jp;
   pthread_rwlock_unlock(&qp->q_lock);
}

void job_remove(struct queue *qp,pthread_t id) {
   pthread_rwlock_wrlock(&qp->q_lock);
   if(jp == qp->head) {
      qp->q_head = jp->j_next;
      if(qp->q_tail == jp) {
         qp->q_tail = NULL;
      }
   } else if(jp == pq->tail) {
     pq->tail = jp->j_prev;
   } else {
     jp->j_prev->j_next = jp->j_next;
     jp->j_next->j_prev = jp->j_prev;
   }

   pthread_rwlock_unlock(&qp->q_lock);
}

struct job * job_find(struct queue *qp,pthread_t id) {
   struct job *jp;
   if(pthread_rwlock_rdlock(&qp->q_lock) != 0) {
      return (NULL);
   }

   for(jp = qp->q_head; jp != NULL; jp = jp->j_next) {
       if(pthread_equal(id,jp->j_id)) {
           break;
       }
   }

   pthread_rwlock_unlock(&qp->q_lock);
   return (jp);
}
