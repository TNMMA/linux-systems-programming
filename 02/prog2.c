#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define BUF_SIZE 16384

int main(void) {
	int fdWrite, fdRead, n;
	char buf[BUF_SIZE];

	if( (fdWrite = open("bar", O_CREAT| O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
		printf("open error with errno = %d\n", errno);
		perror("bar");
		exit(1);
	}
	if( (fdRead = open("foo", O_RDONLY)) == -1) {
		printf("open error with errno = %d\n", errno);
		perror("foo");
		exit(1);
	}

	while( (n=read(fdRead, buf, BUF_SIZE)) > 0) {
		if( (n = write(fdWrite, buf, n)) == -1) {
			printf("write error with errno = %d\n", errno);
			perror("bar");
			exit(1);
		}		
	}

	return 0;
}