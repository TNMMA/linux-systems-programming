#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
  int fdReadWrite;
  size_t n;
  char *records;

  struct record {
    int id;
    char name[80];
  };
  
  struct record *toModify;

  /* Open data file */
  if( (fdReadWrite = open("datafile", O_RDWR)) == -1) {
    printf("open error with errno = %d\n", errno);
    perror("datafile");
    exit(1);
  }

  /* Get size of data file */
  if( (n = lseek(fdReadWrite, 0, SEEK_END)) == -1) {
    printf("lseek error with errno = %d\n", errno);
    perror("datafile");
    exit(1);
  }
  
  /* Memory Map entire data file */
  if( (records = mmap(NULL, n, PROT_READ | PROT_WRITE, MAP_SHARED, fdReadWrite, 0)) == MAP_FAILED) {
    printf("mmap error with errno = %d\n", errno);
    perror("datafile");
    exit(1);
  }
  
  /* Modify the record in memory */
  toModify = (struct record *)records;
  printf("Records before modification:\n");
  printf("Record #0: %d, %s\n", toModify[0].id, toModify[0].name);
  printf("Record #1: %d, %s\n", toModify[1].id, toModify[1].name);
  printf("Record #2: %d, %s\n", toModify[2].id, toModify[2].name);

  toModify[1].id = 0;
      
  printf("Records after modification:\n");
  printf("Record #0: %d, %s\n", toModify[0].id, toModify[0].name);
  printf("Record #1: %d, %s\n", toModify[1].id, toModify[1].name);
  printf("Record #2: %d, %s\n", toModify[2].id, toModify[2].name);

  /* Flush to file */
  if(msync(toModify, n, MS_SYNC) == -1) {
    printf("msync error with errno = %d\n", errno);
    perror("datafile");
    exit(1);
  }
  
  return 0;
}
