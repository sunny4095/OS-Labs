#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    const char* NAME = "shared_mem";
    const int SIZE = 64;
    const char* m = "Shashii";
    int fd_read, fd_write;

    int shm_fd;
    void *ptr;

    mkfifo("fifo1",0666);
    mkfifo("fifo2",0666);

    // write then read
    fd_write = open("fifo1", O_WRONLY);
    fd_read = open("fifo2",O_RDONLY);

    //create shared memory segment and populate it with free identifier
    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd,SIZE);
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    //populate the shared memory with messages and simultaneously send signals to consumer
    char* tmp = ptr;
    for (int i=0; i<8; i++) {
        sprintf(tmp,"%s", m);
        tmp += strlen(m); 
        write(fd_write, &i, sizeof(int));
    }

    //wait for memory to be freed and write back there again
    int tmp1;
    while(1) {
        int offset;
        printf("waiting for consumer ...\n");
        read(fd_read, &offset, sizeof(int));
        if (offset == -1) {
            break;
        }
        char *tmp = ptr + offset * strlen(m);
        sprintf(tmp,"%s", m);
        write(fd_write,&offset,sizeof(int));
        printf("wrote message at offset %d\n",offset);

        // char d_buffer[64];
        // strcpy(d_buffer,ptr);
        // printf("%s\n",d_buffer);
    }

    //cleanup
    shm_unlink(NAME);
    close(fd_write);
    close(fd_read);
    unlink("fifo1");
    unlink("fifo2");
}


