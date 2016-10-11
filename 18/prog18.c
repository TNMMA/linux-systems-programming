#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <fcntl.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

void killtime(void) {
  int i, fd;
   /* Kill Time */
  fd=open("/dev/null", O_RDWR);
  for(i=0; i<100000000; i++) {
    if(write(fd, &i, sizeof(i)) == -1) {
      handle_error("write");
    }
  }
  if(close(fd) == -1) {
    handle_error("close");
  }
  return;
}

int main(int argc, char *argv[]) {
  struct tms processTimes;
  clock_t startTime = clock();
  clock_t endTime;

  printf("%ld\n", sysconf(_SC_CLK_TCK));
  printf("CLOCKS PER SEC: %ld\n", CLOCKS_PER_SEC);

  killtime();
  if(times(&processTimes) == -1) {
    handle_error("times");
  }
  printf("user time = %ld\n", processTimes.tms_utime);
  printf("system time = %ld\n", processTimes.tms_stime);
  printf("child user time = %ld\n", processTimes.tms_cutime);
  printf("child system time = %ld\n", processTimes.tms_cstime);
  endTime = clock();
  startTime=endTime;
  printf("Total time elapsed = %ld\n", endTime-startTime);

  printf("Final clock value = %ld\n", clock()/CLOCKS_PER_SEC);
  return EXIT_SUCCESS;
}
