#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

int main(void) {
	int fdWrite, fdRead;
	size_t n;

	char *read, *write;

	/* Open the read and write files */
	if( (fdWrite = open("bar", O_CREAT| O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
		printf("open error with errno = %d\n", errno);
		perror("bar");
		exit(1);
	}
	if( (fdRead = open("foo", O_RDONLY)) == -1) {
		printf("open error with errno = %d\n", errno);
		perror("foo");
		exit(1);
	}

	/* Get input file size and set output file size */
	n = lseek(fdRead, 0, SEEK_END);
	if(ftruncate(fdWrite, n) == -1) {
		printf("ftruncate error with errno = %d\n", errno);
		perror("bar");
		exit(1);
	}

	/* Create a memory map for the read and write files */
	if( (read = mmap(NULL, n, PROT_READ, MAP_PRIVATE, fdRead, 0)) == MAP_FAILED) {
		printf("mmap error with errno = %d\n", errno);
		perror("foo");
		exit(1);
	}
	if( (write = mmap(NULL, n, PROT_WRITE | PROT_READ, MAP_SHARED, fdWrite, 0)) == MAP_FAILED) {
		printf("mmap error with errno = %d\n", errno);
		perror("bar");
		exit(1);
	}

	/* Copy the contents using memcpy() */
	memcpy(write, read, n);

	/* Flush the memory to file using msync() */
	if(msync(write, n, MS_SYNC) == -1) {
		printf("msync error with errno = %d\n", errno);		
		perror("bar");
		exit(1);
	}

	return 0;
}