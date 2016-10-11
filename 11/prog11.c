#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int getFileSize(char *name) {
  struct stat fileInfo;

  if(stat(name, &fileInfo) == -1) {
    handle_error("stat");
  }
  
  return fileInfo.st_size;
}

int main(int argc, char *argv[]) {
  DIR *directory;
  struct dirent *info;
  int directorySize;

  if(argc != 2) {
    printf("Usage: %s dirname\n", argv[0]);
    exit(EXIT_SUCCESS);
  }
  
  if(chdir(argv[1]) == -1) {
    handle_error("chdir");
  }

  directory = opendir(".");

  directorySize = 0;
  while( (info = readdir(directory)) != NULL) {
    directorySize += getFileSize(info->d_name);
  }
  
  printf("Total size of directory = %d\n", directorySize);

  if(closedir(directory) == -1) {
    handle_error("closedir");
  }

  return EXIT_SUCCESS;
}
