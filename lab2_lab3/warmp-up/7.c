#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char* argv[]) {
    int cpid = fork();
    if (cpid == 0) {
        //child
        sleep(10);
    }
    else {
        // parent
        sleep(5);
        kill(cpid,SIGKILL);
        wait(NULL);
        printf("child reaped\n");
    }
}