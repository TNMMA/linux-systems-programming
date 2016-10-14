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

#define PORT_ADDR 8000
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  int sock;
  struct servent *service_info;
  struct hostent *host_info;
  struct sockaddr_in server;
  int count;
  char buffer[BUF_SIZE];

  /* Create Socket */
  if( (sock=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    handle_error("socket");
  }

  /* Get Server Information */
  /* Assume that /etc/services has been updated with rot13 details */
  service_info = getservbyname("rot13", "tcp");
  host_info = gethostbyname("localhost");

  /* Setup the sockaddr_in for connect call */
  server.sin_family = AF_INET;
  memcpy(&server.sin_addr, host_info->h_addr, host_info->h_length);
  server.sin_port = service_info->s_port;

  /* Connect to Server */
  if(connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
    handle_error("connect");
  }
  
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
