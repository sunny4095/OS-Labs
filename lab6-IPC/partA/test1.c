#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    mkfifo("testfifo2",0666);
    mkfifo("testfifo1",0666);
    // mkfifo("testfifo2",0666);
    int fd_write = open("testfifo2",O_WRONLY);
    int fd_read = open("testfifo1",O_RDONLY);


    
    for (int i=0; i<10; i++) {
        write(fd_write,&i,sizeof(int));
    }

    for (int i=0; i<10; i++) {
        int j;
        read(fd_read, &j,sizeof(int));
        printf("message recieved in test1: %d", j);
    }

    close(fd_read);
    close(fd_write);

    unlink("testfifo1");
    unlink("testfifo2");
}