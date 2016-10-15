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

#define BUF_SIZE           516
#define TFTP_SERVICE       "tftp"
#define TFTP_OPCODE_RRQ    1
#define TFTP_OPCODE_WRQ    2
#define TFTP_OPCODE_DATA   3
#define TFTP_OPCODE_ACK    4
#define TFTP_OPCODE_ERROR  5
#define TFTP_TRANSFER_MODE "octet"

void handle_tftp_read(int sock, struct sockaddr *server, socklen_t serverLen, char *filename) {
  char buffer[BUF_SIZE];
  char *p = buffer;
  int payloadLength;
  int count;
  struct sockaddr_in servRespAddr;
  socklen_t servRespAddrLen;
  short opcode;
  short blockNumber;
  
  /* First Generate a Payload for Read Request 
   *  _________ ____________ _____________
   * | Opcode  | File Name  |  Mode       |
   * | 2 bytes | (string)   |  (string)   |
   * --------------------------------------
   */
  opcode = htons(TFTP_OPCODE_RRQ);
  memcpy(p, &opcode, sizeof(short));
  p = p + 2;
  p = strcpy(p, filename);
  p = p + strlen(filename) + 1; /* account for null terminator */
  p = strcpy(p, TFTP_TRANSFER_MODE);
  p = p + strlen(TFTP_TRANSFER_MODE) + 1; /* account for null terminator */
  payloadLength = p - buffer;

  /* Send the Read Request */
  if(sendto(sock, buffer, payloadLength, 0, server, serverLen) == -1) {
    handle_error("sendto");
  }

  /* Fall into the loop to handle DATA and ERROR till end of transmission*/
  do {
    servRespAddrLen = sizeof(struct sockaddr_in);
    /* Read Data OR Error
     *  _________ ____________ _____________
     * | Opcode  | Block Num  |  Data       |
     * | 2 bytes |  2 bytes   |  512 bytes  |
     * --------------------------------------
     */
    if( (count = recvfrom(sock, buffer, BUF_SIZE, 0, (struct sockaddr*)&servRespAddr, &servRespAddrLen)) == -1) {
      handle_error("recvfrom");
    }
    memcpy(&opcode, buffer, sizeof(short));
    memcpy(&blockNumber, buffer+2, sizeof(short));
    opcode = ntohs(opcode);
    blockNumber = ntohs(blockNumber);

    switch(opcode) {
    case TFTP_OPCODE_ERROR:
      fprintf(stderr, "TFTP server error code %d: %s\n", blockNumber, buffer+4);
      exit(EXIT_FAILURE);
      break;
    case TFTP_OPCODE_DATA:
      /* Stream data to stdout */
      if(write(1, buffer+4, count-4) == -1) {
        handle_error("write");
      }
      /* Send the ACK Request */
      /* Copy only ACK Opcode as Block Number is already in place */
      opcode = htons(TFTP_OPCODE_ACK);
      memcpy(buffer, &opcode, sizeof(short));
      payloadLength = 4;
      if(sendto(sock, buffer, payloadLength, 0, (struct sockaddr *)&servRespAddr, sizeof(struct sockaddr_in)) == -1) {
        handle_error("sendto");
      }
      break;
    default:
      fprintf(stderr, "Unknown opcode from the server\n");
      break;
    }
  } while(count == 516); /*Indicates more data is available to read */
  
  return;
}

int main(int argc, char *argv[]) {
  int sock;
  struct addrinfo hints;
  struct addrinfo *results, *rp, *server;
  int retval;

  if(argc < 3) {
    printf("Usage: %s <server-name> <filename>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Get Server Information */
  /* Assume that /etc/services has been updated with rot13 details */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; /* try only IPv4 */
  hints.ai_socktype = SOCK_DGRAM; /* only UDP connections */
  if( (retval = getaddrinfo(argv[1], TFTP_SERVICE, &hints, &results)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(retval));
    exit(EXIT_FAILURE);
  }

  /* Traverse each result entry and try to create a socket until success */
  server = NULL;
  rp = results; 
  while(rp != NULL) {
    /* Try to create socket */
    if( (sock=socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) != -1) {
      /* Success creating socket */
      server = rp;
      rp = NULL;
    } else {
      printf("Failed socket creation, moving to next\n");
      /* Unable to create socket, move to next connection */
      rp = rp->ai_next;
    }
  }
  
  if(server == NULL) {
    fprintf(stderr, "Unable to connect to server %s:%s\n", argv[1], argv[2]);
    exit(EXIT_FAILURE);
  } 

  /* Handle Connection: */
  handle_tftp_read(sock, server->ai_addr, server->ai_addrlen, argv[2]);

  /* Clean up before exiting */
  freeaddrinfo(results);
  close(sock);
  return EXIT_SUCCESS;
}
