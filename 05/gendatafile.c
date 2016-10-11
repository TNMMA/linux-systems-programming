#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int fdWrite, n;

	struct record {
		int id;
		char name[80];
	};

	struct record ashish[3] = {
		{ 12345, "ashish" },
		{ 54321, "divya" },
		{ 543210, "rudra" }
	};

	char buffer[3*84];
	memcpy(&buffer[0*84], &ashish[0], 84);
	memcpy(&buffer[1*84], &ashish[1], 84);
	memcpy(&buffer[2*84], &ashish[2], 84);

	if( (fdWrite = open("datafile", O_CREAT| O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
		printf("open error with errno = %d\n", errno);
		perror("datafile");
		exit(1);
	}

	if( (n = write(fdWrite, buffer, 84*3)) == -1) {
		printf("write error with errno = %d\n", errno);
		perror("datafile");
		exit(1);
	}

	return 0;
}