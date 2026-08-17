#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main(){
	const char* fifo = "/tmp/fifo";
	mkfifo(fifo, 0666);
	int fd = open(fifo, O_WRONLY);

	// char* hello = "hello";
	// write(fd, hello, strlen(hello) + 1);
	char buffer[64];
	while(1){
		printf("Enter a message: ");
		// take the input
		fgets(buffer, 64, stdin);
		write(fd, buffer, strlen(buffer) + 1);
	}
	close(fd);
	return 0;
}
