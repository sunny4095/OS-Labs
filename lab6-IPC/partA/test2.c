#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    int fd_read = open("testfifo2",O_RDONLY);
    int fd_write = open("testfifo1",O_WRONLY);

    for (int i=0; i<10; i++) {
        int j;
        read(fd_read, &j,sizeof(int));
        printf("message recieved in test2: %d", j);
    }
    sleep(2);
    for (int i=0; i<10; i++) {
        write(fd_write,&i,sizeof(int));
    }
    close(fd_read);
    close(fd_write);
}