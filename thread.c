#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "benchmark.h"

#define N 42

void *thread(void * param) {
  sleep(1); // pas un `while (true)` comme ça le thread est pas "ready"
  pthread_exit(NULL);
}

int main (int argc, char *argv[])  {
  pthread_t threads[N];
  int err = 0, i;

  timer *t = timer_alloc();
  recorder *create_rec = recorder_alloc("create.csv");
  recorder *join_rec = recorder_alloc("join.csv");

  // BEGIN
  for (i = 0; i < N; i++) {
    start_timer(t);
    err = pthread_create(&threads[i], NULL, &thread, NULL);
    write_record(create_rec, i, stop_timer(t));
    if (err != 0)
      error(1, err, "pthread_create");
  }
  // END

  sleep(2); // Pour s'assurer que les pthread_join ne patientent pas

  // BEGIN
  for (i = 0; i < N; i++) {
    start_timer(t);
    err = pthread_join(threads[i], NULL);
    write_record(join_rec, i, stop_timer(t));
    if (err != 0)
      error(1, err, "pthread_join");
  }
  // END

  recorder_free(join_rec);
  recorder_free(create_rec);
  timer_free(t);

  return EXIT_SUCCESS;
}