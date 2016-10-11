#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

void printFileDetails(char *name) {
  struct stat fileInfo;
  char r;
  char *modtime;

  if(stat(name, &fileInfo) == -1) {
    handle_error("stat");
  }
  
  if(S_ISREG(fileInfo.st_mode)) { r = '-'; }
  if(S_ISDIR(fileInfo.st_mode)) { r = 'd'; }
  if(S_ISCHR(fileInfo.st_mode)) { r = 'c'; }
  if(S_ISBLK(fileInfo.st_mode)) { r = 'b'; }
  if(S_ISFIFO(fileInfo.st_mode)) { r = 'p'; }
  if(S_ISLNK(fileInfo.st_mode)) { r = 'l'; }
  if(S_ISSOCK(fileInfo.st_mode)) { r = 's'; }

  printf("%c%c%c%c%c%c%c%c%c%c", r,
         (fileInfo.st_mode & S_IRUSR)? 'r' : '-',
         (fileInfo.st_mode & S_IWUSR)? 'w' : '-',
         (fileInfo.st_mode & S_IXUSR)? 'x' : '-',
         (fileInfo.st_mode & S_IRGRP)? 'r' : '-',
         (fileInfo.st_mode & S_IWGRP)? 'w' : '-',
         (fileInfo.st_mode & S_IXGRP)? 'x' : '-',
         (fileInfo.st_mode & S_IROTH)? 'r' : '-',
         (fileInfo.st_mode & S_IWOTH)? 'w' : '-',
         (fileInfo.st_mode & S_IXOTH)? 'x' : '-');

  printf(" %8ld",fileInfo.st_size);
  modtime = ctime(&fileInfo.st_mtime);
  /* remove the '\n' at the end of modtime */
  modtime[strlen(modtime) -1] = '\0';

  printf(" %s", modtime);
  printf(" %s\n", name);
  
  return;
}

int main(int argc, char *argv[]) {

  if(argc != 2) {
    printf("Usage: %s filename\n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  printFileDetails(argv[1]);
  return EXIT_SUCCESS;
}
