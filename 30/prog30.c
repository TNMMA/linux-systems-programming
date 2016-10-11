#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int count;
char val;

void sighupHandler(int sig) {
  FILE *fp;
  if( (fp=fopen("reconfigure.conf", "r")) == NULL) {
    handle_error("fopen");
  }
  if(fscanf(fp, "%c %d\n", &val, &count)) {
    if(ferror(fp)) {
      handle_error("fscanf");
    }
  }
  fclose(fp);
  return;
}


int main(int argc, char *argv[]) {
  sigset_t sigmask;
  struct sigaction action;
  int i;

  printf("Process id=%d\n", getpid());

  /* Handle SIGINT */
  action.sa_handler = sighupHandler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;
  sigaction(SIGHUP, &action, NULL);

  /* Setup sigmask to handle critical region */
  sigemptyset(&sigmask);
  sigaddset(&sigmask, SIGHUP);

  /* Kick start the process */
  sighupHandler(1);
  while(1) {
    sleep(1);
    sigprocmask(SIG_BLOCK, &sigmask, NULL);
    for(i=0; i<count; i++) {
      printf("%c", val);
    }
    sigprocmask(SIG_UNBLOCK, &sigmask, NULL);
    printf("\n");
  }
  return EXIT_SUCCESS;
}
