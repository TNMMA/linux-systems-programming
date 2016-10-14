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
void connectionHandler(int fdInput, int fdOutput) {
  char input[BUF_SIZE];
  char output[BUF_SIZE];
  int readSize;

  do {
    if( (readSize=read(fdInput, input, BUF_SIZE)) == -1) {
      handle_error("read");
    }
    /* Process and Respond to data */
    if(readSize > 0) {
      rot13((unsigned char*)input, (unsigned char*)output, readSize);
      if(write(fdOutput, output, readSize) == -1) {
        handle_error("write");
      }
    }
  } while(readSize); /* readSize = 0 when client closes connection */

  return;
}

int main(int argc, char *argv[]) {
  int sock, fd;
  socklen_t clientLen;
  struct sockaddr_in server, client;

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
  
  while(1) {
    /* Accept Connections, blocks till connection is established */
    clientLen = sizeof(client);
    if( (fd=accept(sock, (struct sockaddr *)&client, (socklen_t *)&clientLen)) == -1) {
      handle_error("accept");
    }

    /* Handle Connection */
    printf("Got new connection from client: %d\n", client.sin_addr.s_addr);
    connectionHandler(fd, fd);
    
    /* Very important to close file descriptors */
    printf("Client closed connection, so server closing connection\n");
    close(fd); 
  }

  return EXIT_SUCCESS;
}
