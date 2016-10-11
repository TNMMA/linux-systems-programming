#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

extern char *optarg;
extern int optind, opterr, optopt;

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
  char opt;
  int cFlag=0, vFlag=0, bFlag=0, fFlag=0;
  int bOpt=0;
  char *fOpt=NULL;
  int i=0,j=0;

  while( (opt=getopt(argc, argv, "cvb:f:")) != -1) {
    switch(opt) {
    case 'c':
      cFlag=1;
      break;
    case 'v':
      vFlag=1;
      break;
    case 'b':
      bFlag=1;
      bOpt=atoi(optarg);
      break;
    case 'f':
      fFlag=1;
      if( (fOpt=malloc(strlen(optarg)*sizeof(char))) == NULL) {
        handle_error("malloc failed for fOpt");
      }
      strcpy(fOpt, optarg);
      break;
    default:
      exit(EXIT_FAILURE);
    }
  }
  
  if(cFlag) { printf("Found option c\n"); }
  if(vFlag) { printf("Found option v\n"); }
  if(bFlag) { printf("Found option b with value: %d\n", bOpt); }
  if(fFlag) { printf("Found option f with value: %s\n", fOpt); }

  /* Print all the non-option arguments */
  i=optind; /*the index of non-option arguments */
  j=argc-optind; /*number of non-option arguments */

  while(j) {
    printf("Found non-option argument: %s\n", argv[i]);
    i++; j--;
  }

  free(fOpt);
  return EXIT_SUCCESS;
}
