#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int count;
char val;

void sigalrmHandler(int sig) {
  /* Do nothing */
  return;
}


int main(int argc, char *argv[]) {
  struct sigaction action;
  int i;
  char line[100];
  printf("Process id=%d\n", getpid());

  /* Handle SIGINT */
  action.sa_handler = sigalrmHandler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0; /* To ensure the blocking read times out */
  sigaction(SIGALRM, &action, NULL);

  /* Kick start the process */
  while(1) {
    printf("Enter a number: ");
    fflush(stdout); /* force the prompt to the screen */
    alarm(5); /* setup a 5 second timeout */
    i = read(0, &line, 100);
    alarm(0); /* clear the timeout */
    if(i == -1) {
      printf("timed out!\n");
    } else {
      printf("got input = %d\n", atoi(line));
    }
  }
  return EXIT_SUCCESS;
}
