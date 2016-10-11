#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)


int main(int argc, char *argv[]) {
  FILE *conf;
  int notifyHandle;
  int i;

  /* Linux specified max filename is: NAME_MAX */
  char line[NAME_MAX+1]; 

  /* Handle 100 events at one go      */
  int watchHandles[100];
  char eventBuffer[100*sizeof(struct inotify_event) + NAME_MAX+1];
  char *p;
  struct inotify_event *eventPtr;
  

  /* Create a notifyHandle to watch file system events */
  if( (notifyHandle = inotify_init()) == -1) {
    handle_error("inotify_init()");
  }

  /* Open monitor.conf file */
  if( (conf = fopen("monitor.conf", "r")) == NULL) {
    handle_error("monitor.conf fopen");
  }

  /* create individual watchHandles and add it to notifyHandle*/
  i = 0;
  while(fgets(line, NAME_MAX+1, conf) != NULL) {
    /* Remove \n from the line read */
    line[strlen(line)-1] = '\0';
    if( (watchHandles[i++] = inotify_add_watch(notifyHandle, line, IN_DELETE_SELF | IN_MODIFY)) == -1) {
      handle_error(line);
    }
  }

  while(1) {
    if( (i = read(notifyHandle, eventBuffer, sizeof(eventBuffer))) == -1) {
      handle_error("read notifyHandle");
    }
    
    /* Iterate over all the events and print each one */
    p = eventBuffer;
    while(p < eventBuffer+i) {
      eventPtr = (struct inotify_event *)p;
      if(eventPtr->mask & IN_MODIFY) {
        printf("Got modify event on filehandle: %d\n", eventPtr->wd);
      }
      if(eventPtr->mask & IN_DELETE_SELF) {
        printf("Got delete event on filehandle: %d\n", eventPtr->wd);
      }
      p += sizeof(struct inotify_event) + eventPtr->len;
    }
  }

  return EXIT_SUCCESS;
}
