#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <getopt.h>
#include <sys/stat.h>

extern char *optarg;
extern int optind, opterr, optopt;

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int aFlag=0, rFlag=0;
char dirToParse[PATH_MAX];
int hist[24];

void parseInputs(int argc, char*argv[]) {
  char opt;

  while( (opt=getopt(argc, argv, "ar")) != -1) {
    switch(opt) {
    case 'r':
      rFlag=1;
      break;
    case 'a':
      aFlag=1;
      break;
    default:
      printf("Usage: %s -a -r directory\n", argv[0]);
      printf("-a: ignore hidden files\n-r: recurse\n");
      exit(EXIT_FAILURE);
    }
  }
  
  if(!argv[optind]) {
    printf("Missing directory\n");
    printf("Usage: %s -a -r directory\n", argv[0]);
    printf("-a: ignore hidden files\n-r: recurse\n");
    exit(EXIT_FAILURE);
  }

  printf("%d\n", PATH_MAX);
  /* Directory name is first non-option argument */
  /* Create absolute path of directory */
  if(argv[optind][0] == '/') {
    strncpy(dirToParse, argv[optind], PATH_MAX);
  } else {
    if(getcwd(dirToParse, PATH_MAX) == NULL) {
      handle_error("getcwd");
    }
    printf("curr working dir = %s\n", dirToParse);
    strcat(dirToParse, "/");
    if( strlen(dirToParse) + strlen(argv[optind]) < PATH_MAX-1) {
      strcat(dirToParse, argv[optind]);
    } else {
      printf("Directory name too long\n");
      exit(EXIT_FAILURE);
    }
  }

  return;
}

void printFileDetails(struct stat fileInfo, char *name) {
  char r;
  struct tm *modtime;

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
  modtime = localtime(&fileInfo.st_mtime);

  printf(" %d", modtime->tm_hour);
  printf(" %s\n", name);
  
  hist[modtime->tm_hour]++;
  return;
}

void processDirectory(char *dirname) {
  DIR *directory;
  struct dirent *info;
  struct stat fileInfo;
  char fullFileName[PATH_MAX];

  if( (directory = opendir(dirname)) == NULL) {
    handle_error("opendir");
  }
    
  while( (info = readdir(directory)) != NULL) {
    /* Skip hidden files if !aFlag */
    if( (info->d_name[0] == '.') && (!aFlag) ) { 
      continue; 
    }

    /* Get stat to distinguish files from directories */
    /* stat requires Absolute Path of Directory */
    strcpy(fullFileName, dirname);
    strcat(fullFileName, "/");
    strcat(fullFileName, info->d_name);
    if(stat(fullFileName, &fileInfo) == -1) {
      handle_error("stat");
    }

    if(rFlag && S_ISDIR(fileInfo.st_mode)) {
      processDirectory(fullFileName);
    } else { 
      printf("parsing %s\n",info->d_name);
      printFileDetails(fileInfo, info->d_name);
    }
  }
  
  if(closedir(directory) == -1) {
    handle_error("closedir");
  }

}

int main(int argc, char *argv[]) {
  int i;

  for(i=0; i<24; i++) {
    hist[i] = 0;
  }
  
  parseInputs(argc, argv);
  if(aFlag) { printf("Found option a\n"); }
  if(rFlag) { printf("Found option r\n"); }
  printf("Found directory to parse %s\n", dirToParse);

  processDirectory(dirToParse);

  for(i=0; i<24; i++) {
    printf("Hour %d: %d\n", i, hist[i]);
  }
  
  return EXIT_SUCCESS;
}
