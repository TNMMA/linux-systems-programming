#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define PORT_ADDR 8000
#define BUF_SIZE 1024

/* 
 * Inplace Transform the input data
 */
void rot13(unsigned char *input, unsigned char *output, int length) {
  /* Ignore \r\n carriage return and line feed from the stream */
  length = length - 2; 
  while(length--) {
    /* Dont rotate space */
    if(*input == ' ') {
      *output = ' ';
    } else {
      *output = *input + 13;
      if(*output > 'z') {
        *output = *output - 26;
      }
    }
    input++;
    output++;
  }
  /* Copy over carriage return and line feed */
  *output++ = *input++;
  *output = *input;
  return;
}

/*
 * Manage Connection
 */
int connectionHandler(int fdInput, int fdOutput) {
  char input[BUF_SIZE];
  char output[BUF_SIZE];
  int readSize;
  
  if( (readSize=read(fdInput, input, BUF_SIZE)) < 1) {
    /* On error (readSize == -1) OR client connection close (readSize == 0) */
    return -1;
  }
  /* Process and Respond to data */
  rot13((unsigned char*)input, (unsigned char*)output, readSize);
  if(write(fdOutput, output, readSize) < 1) {
    /* On error OR client close of connection */
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int sock, fd;
  socklen_t clientLen;
  struct sockaddr_in server, client;
  fd_set fullClientList, activeClientList;
  int i;

  FD_ZERO(&fullClientList);
  FD_ZERO(&activeClientList);
  printf("FD_SETSIZE (number of maximum clients this server can monitor) = %d\n", FD_SETSIZE);

  /* Create a socket */
  if( (sock=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    handle_error("socket");
  }

  /* Bind Socket to Port 8000 */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORT_ADDR);

  if(bind(sock, (struct sockaddr *)&server, (socklen_t)sizeof(server)) == -1) {
    handle_error("bind");
  }
  
  /* Setup Listen Queue */
  if(listen(sock, 5) == -1) {
    handle_error("listen");
  }
  printf("Listening on port %d\n", PORT_ADDR);

  /* Add socket into fullClientList */
  FD_SET(sock, &fullClientList);
  while(1) {
    /* duplicate the full client list as select will overwrite it */
    memcpy(&activeClientList, &fullClientList, sizeof(fullClientList));
    /* blocks till any connection is ready OR established */
    select(FD_SETSIZE, &activeClientList, NULL, NULL, NULL);
    
    /* First check sock descriptor, and accept the connection */
    if(FD_ISSET(sock, &activeClientList)) {
      clientLen = sizeof(client);
      if( (fd=accept(sock, (struct sockaddr *)&client, (socklen_t *)&clientLen)) == -1) {
        handle_error("accept");
      }
      /* Add fd to the fullClientList to keep listening on it */
      FD_SET(fd, &fullClientList);
      printf("Got new connection from client: %d\n", client.sin_addr.s_addr);
    }
    
    /* Handle All Other Client Connections */
    /* ignore stdin, stdout, stderr and sock */
    for(i=3; i<FD_SETSIZE; i++) {
      if(i != sock) { 
        if(FD_ISSET(i, &activeClientList)) {
          if(connectionHandler(i, i) == -1) {
            /* connection closed OR error so stop monitoring this */
            /* Very important to close file descriptors */
            printf("Client closed connection, so server closing connection\n");
            close(i); 
            FD_CLR(i, &fullClientList);
          }
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
