#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {
    const int SIZE = 4096;
    const char *name = "shared_mem_segment";
    const char *message0 = "Hello ";
    const char *message1 = "I ";
    const char *message2 = "am ";
    const char *message3 = "sunny ";

    int shm_fd;
    void *ptr;

    // create the shared memory segment
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    //configure the size of the shared menmory segment
    ftruncate(shm_fd,SIZE);

    // map the shared segment into address space
    ptr = mmap(0,SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,0);
     
    //write into shared memory
    sprintf(ptr,"%s", message0);
    ptr += strlen(message0);
    sprintf(ptr,"%s", message1);
    ptr += strlen(message1);
    sprintf(ptr,"%s", message2);
    ptr += strlen(message2);
    sprintf(ptr,"%s", message3);
    
}
