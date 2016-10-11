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
  struct sigaction action;
  int numBytesRead;
  char buffer[10000];
  int p[2];

  printf("Process id=%d\n", getpid());

  /* Handle SIGINT */
  action.sa_handler = sigintHandler;
  sigemptyset(&action.sa_mask);
  /*action.sa_flags = SA_RESTART;*/
  action.sa_flags = 0;
  sigaction(SIGINT, &action, NULL);
  
  if(pipe(p) == -1) {
    handle_error("pipe");
  }
  while(1) {
    /* Blocking read as there is no writer to the pipe */
    if( (numBytesRead=read(p[0], buffer, 1000)) == -1) {
      /*handle_error("read");*/
      /* ignore error for demo */
    }
    printf("got %d number of bytes, read input = %s\n", numBytesRead, buffer);
  }
  return EXIT_SUCCESS;
}
