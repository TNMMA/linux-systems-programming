#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int fdReadWrite, n;
	int recordSize;
	struct record {
		int id;
		char name[80];
	};

	struct record toModify;
	recordSize = sizeof(struct record);


	if( (fdReadWrite = open("datafile", O_RDWR)) == -1) {
		printf("open error with errno = %d\n", errno);
		perror("datafile");
		exit(1);
	}

	if( (n=lseek(fdReadWrite, recordSize, SEEK_SET)) == -1) {
		printf("lseek error with errno = %d\n", errno);
		perror("datafile");
		exit(1);
	}

	if( (n = read(fdReadWrite, &toModify, recordSize)) == -1) {
		printf("read error with errno = %d\n", errno);
		perror("datafile");
		exit(1);
	}

	printf("About to modify: %d, %s\n", toModify.id, toModify.name);

	toModify.id = 56789;

	/* Rewind to update the record in correct position */
	if( (n=lseek(fdReadWrite, -1*recordSize, SEEK_CUR)) == -1) {
		printf("lseek error with errno = %d\n", errno);
		perror("datafile");
		exit(1);
	}

	if( (n = write(fdReadWrite, &toModify, recordSize)) == -1) {
		printf("write error with errno = %d\n", errno);
		perror("datafile");
		exit(1);
	}


	return 0;
}