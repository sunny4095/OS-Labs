#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


int main() {
    const char* NAME = "shared_mem";
    const char* fi = "ffrreee";
    const int SIZE = 64;
    int shm_fd;
    int fd_write, fd_read;
    void *ptr;

    shm_fd = shm_open(NAME, O_RDWR, 0666);
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // read and then write
    fd_read = open("fifo1",O_RDONLY);
    fd_write = open("fifo2",O_WRONLY);

    int strings_consumed = 0;

    int tmp1;
    while (1) {
        int offset;
        read(fd_read,&offset,sizeof(int));
        char *tmp = ptr + offset * strlen(fi);
        char message[8];
        strcpy(message,tmp);
        printf("message read at offset %d : %s\n", offset, message);
        sprintf(tmp,"%s",fi);
        sleep(1);
        write(fd_write,&offset,sizeof(int));
        strings_consumed++;
        if (strings_consumed == 10) {break;}
    }

    tmp1 = -1;
    write(fd_write, &tmp1,sizeof(int));
    close(fd_write);
    close(fd_read); 
}

