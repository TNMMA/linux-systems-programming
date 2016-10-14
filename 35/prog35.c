#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  int sock;
  struct addrinfo hints;
  struct addrinfo *results, *rp;
  int count, retval, success;
  char buffer[BUF_SIZE];

  if(argc < 3) {
    printf("Usage: %s <server-name> <service-name/port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Get Server Information */
  /* Assume that /etc/services has been updated with rot13 details */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC; /* try both IPv4 and IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* only tcp connections */
  if( (retval = getaddrinfo(argv[1], argv[2], &hints, &results)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(retval));
    exit(EXIT_FAILURE);
  }

  /* Traverse each result entry and try to connect until success */
  success = 0;
  rp = results; 
  while(rp != NULL) {
    /* Try to create socket */
    if( (sock=socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) != -1) {
      /* Success creating socket, try to connect */
      if(connect(sock, (struct sockaddr *)rp->ai_addr, rp->ai_addrlen) != -1) {
        /* Successful connect to the server, break out of the loop */
        printf("Successfully connected to %s:%s\n", argv[1], argv[2]);
        success = 1;
        rp = NULL;
      } else {
        /* Unable to connect, close the socket connection */
        rp = rp->ai_next;
        close(sock);
      }
    } else {
      /* Unable to create socket, move to next connection */
      rp = rp->ai_next;
    }
  }
  
  if(!success) {
    fprintf(stderr, "Unable to connect to server %s:%s\n", argv[1], argv[2]);
    exit(EXIT_FAILURE);
  } 

  /* Free the results */
  freeaddrinfo(results);
  
  /* Handle Connection: */ 
  /* read from stdin, tx to server, rx from server, print to stdout */
  while(1) {
    if( (count=read(0, buffer, BUF_SIZE)) == -1) {
      handle_error("read");
    }
    if(write(sock, buffer, count) == -1) {
      handle_error("write");
    }
    if( (count=read(sock, buffer, count)) == -1) {
      handle_error("read");
    }
    if(write(1, buffer, count) == -1) {
      handle_error("write");
    }
  }
  
  return EXIT_SUCCESS;
}
