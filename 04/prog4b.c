#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int fdWrite, n;
	char *buf;
	int blocksize, blockcount;

	if(argc != 3) {
		printf("Usage: %s blocksize blockcount\n", argv[0]);
		exit(1);
	}

	blocksize = atoi(argv[1]);
	blockcount = atoi(argv[2]);

	if( (buf  = malloc(blocksize*(sizeof(char)))) == NULL) {
		printf("%s\n", "error allocating blocksize, out of heap memory");
		exit(1);
	}

	if( (fdWrite = open("bar", O_CREAT| O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
		printf("open error with errno = %d\n", errno);
		perror("bar");
		exit(1);
	}

	while( blockcount > 0) {
		if( (n = write(fdWrite, buf, blocksize)) == -1) {
			printf("write error with errno = %d\n", errno);
			perror("bar");
			exit(1);
		}
		blockcount--;		
	}

	return 0;
}