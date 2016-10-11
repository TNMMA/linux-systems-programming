#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
  time_t datetime;
  struct tm *lt;
  char buffer[1000];

  datetime=time(NULL);
  if(!datetime) {
    handle_error("time");
  }
  
  lt = localtime(&datetime);
  strftime(buffer, 1000, "%R %A %e %B", lt);

  printf("The time returned by ctime is %s\n", ctime(&datetime));
  printf("The time returned by localtime is %s\n", buffer);
  return EXIT_SUCCESS;
}
