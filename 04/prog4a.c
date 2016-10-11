#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	FILE *fdWrite;
	char *buf;
	int blocksize, blockcount, n;

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

	if( (fdWrite = fopen("bar", "w")) == NULL) {
		printf("fopen error with errno = %d\n", errno);
		perror("bar");
		exit(1);
	}

	while( blockcount > 0) {
		if( (n = fwrite(buf, 1, blocksize, fdWrite)) == 0) {
			if(ferror(fdWrite)) {
				printf("bar write error with errno = %d\n", ferror(fdWrite));
				exit(1);
			}
		}
		blockcount--;		
	}

	return 0;
}