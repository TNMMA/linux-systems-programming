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

#define BUF_SIZE           1024
#define BROADCAST_SERVICE  8010

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in server, client;
  socklen_t clientAddrLen; 
  int yes = 1;
  int count;
  char buffer[BUF_SIZE];
  
  /* Create a socket */
  if( (sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    handle_error("socket");
  }
  
  /* Setup socket for broadcast messages */
  if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes)) == -1) {
    handle_error("setsockopt");
  }

  /* Bind socket to UDP Broadcast Host Addr and Port */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(BROADCAST_SERVICE);
  if(bind(sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
    handle_error("bind");
  }
  
  if(fork() == 0) {
    /* fork() a child that listens to broadcast messages*/
    while(1) {
      /* Receive Broadcast Messages */
      clientAddrLen = sizeof(struct sockaddr_in);
      if( (count = recvfrom(sock, buffer, BUF_SIZE, 0, (struct sockaddr*)&client, &clientAddrLen)) == -1) {
        handle_error("recvfrom");
      }
      /* Print them on the stdout */
      if(write(1, buffer, count) == -1) {
        handle_error("write");
      }
    }
  } else {
    /* Parent */
    while(1) {
      /* Scan data from stdin */
      if( (count = read(0, buffer, BUF_SIZE)) == -1) {
        handle_error("read");
      }
      /* Broadcast the data */
      server.sin_family = AF_INET;
      server.sin_port = htons(BROADCAST_SERVICE);
      server.sin_addr.s_addr = 0xFFFFFFFF;
      if(sendto(sock, buffer, count, 0, (struct sockaddr*)&server, sizeof(server)) == -1) {
        handle_error("sendto");
      }
    }
  }
  return EXIT_SUCCESS;
}
