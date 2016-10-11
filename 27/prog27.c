#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int counter = 0;

void sigintHandler(int sig) {
  counter = 0;
  return;
}

int main(int argc, char *argv[]) {
  sigset_t sigmask;
  struct sigaction action;
  
  printf("Process id=%d\n", getpid());

  /* Handle SIGINT */
  action.sa_handler = sigintHandler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;
  sigaction(SIGINT, &action, NULL);
  
  /* IGNORE SIGQUIT */
  /* SIGKILL cannot be ignored, it will kill the process */
  action.sa_handler = SIG_IGN;
  sigaction(SIGQUIT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  
  sigemptyset(&sigmask);
  sigaddset(&sigmask, SIGINT);
  while(1) {
    sleep(1);
    printf("Counting = %d\n", counter);
    /* Critical region, cannot be interrupted */
    sigprocmask(SIG_BLOCK, &sigmask, NULL);
    counter++;
    sigprocmask(SIG_UNBLOCK, &sigmask, NULL);
  }
  return EXIT_SUCCESS;
}
