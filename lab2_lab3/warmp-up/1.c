#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>

int main(int argc, char* argv[]) {
    int rc = fork();
    if (rc == 0) {
        // child process
        printf("I am Child of pid %d\n", (int) getppid());
        exit(1);

    } else {
        // parent enters here
        int pid_of_dead_child = wait(NULL);
        printf("I am the parent, pid %d\n", (int) getpid());
        printf("%d\n", (int) pid_of_dead_child);
    }
    return 0;
}