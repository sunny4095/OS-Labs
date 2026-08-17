#include <stdio.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    const char *name = "shared_mem_segment";
    const int SIZE = 4096;

    int shm_fd;
    void *ptr;
    int i;

    // open the shared memory segment
    shm_fd = shm_open(name,O_RDONLY,0666);

    // map the shared memory segment in the address space of the process
    ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    //read from the shared memory region
    printf("%s", (char *)ptr);

    //remove the shared memory segment
    shm_unlink(name);

}