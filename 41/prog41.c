#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

void *child_thread(void *args) {
  printf("I am in child thread, starting msg = %s\n", (char*)args);
  sleep(10);
  return ((void *)99);
}

int main(void) {
  int exitStatus, n;
  pthread_t child;
  if( (n = pthread_create(&child, NULL, child_thread, "computer vision")) != 0) {
    fprintf(stderr, "pthread_create(%d): unable to create thread\n", n);
    exit(EXIT_FAILURE);
  }
  printf("I am in parent thread, waiting for child\n");
  pthread_join(child, (void **)&exitStatus);
  printf("Child exited with code = %d\n", exitStatus);
  return EXIT_SUCCESS;
}
