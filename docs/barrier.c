#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define NTHREADS 10

pthread_mutex_t m;
pthread_cond_t cond_var;

int count;
int number = NTHREADS;

void wait_for_barrier(int i) {
  pthread_mutex_lock(&m);
  if (++count == number) {
    printf("everybody's here, notifying CV!\n");
    pthread_cond_broadcast(&cond_var);
    count = 0;
  } else {
    printf("thread %d waiting at barrier, # waiters = %d\n", i, count);
    pthread_cond_wait(&cond_var, &m);
  }
  pthread_mutex_unlock(&m);

  printf("thread %d is past the barrier!\n", i);

  sleep(5);

  wait_for_barrier(i);
}

void go(int i) {
  printf("thread %d starting!\n", i);
  wait_for_barrier(i);
}

int main() {
  pthread_t threads[NTHREADS];

  pthread_mutex_init(&m, NULL);
  pthread_cond_init(&cond_var, NULL);

  for (int i = 0; i < NTHREADS; i++) {
    pthread_create(&threads[i], NULL, go, i);
  }
  for (int i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
}
