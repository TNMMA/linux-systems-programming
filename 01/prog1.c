#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(void) {
	int fd, n;

	if( (fd = open("hello.txt", O_TRUNC | O_CREAT| O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
		printf("open error with errno = %d\n", errno);
		perror("hello.txt");
		exit(1);
	}
	printf("%d\n", fd);
	if( (n = write(fd, "hello world\n", 12)) == -1) {
		printf("write error with errno = %d\n", errno);
		perror("hello.txt");
		exit(1);
	}
	return 0;
}