#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define BUF_SIZE 16384

int main(void) {
	FILE *fdWrite, *fdRead;
	int n;
	char buf[BUF_SIZE];

	if( (fdWrite = fopen("bar", "w")) == NULL) {
		printf("open error with errno = %d\n", errno);
		perror("bar");
		exit(1);
	}
	if( (fdRead = fopen("foo", "r")) == NULL) {
		printf("open error with errno = %d\n", errno);
		perror("foo");
		exit(1);
	}

	while( (n=fread(buf, 1, BUF_SIZE, fdRead)) > 0) {
		if( (n = fwrite(buf, 1, n, fdWrite)) == 0) {
			if(ferror(fdWrite)) {
				printf("bar write error with errno = %d\n", ferror(fdWrite));
				exit(1);
			}
		}		
	}

	return 0;
}