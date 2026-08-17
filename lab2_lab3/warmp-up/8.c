#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

void sigint_handler() {
    printf("I won't stop\n");
    return ;
}

int main (int argc, char* argv[]) {

    int cpid = fork();
    if (cpid == 0) {
        //child
        signal(SIGINT,sigint_handler);
        printf("Child PID : %d\n", (int) getpid());
        while(1) {
            printf("running\n");
            sleep(1);
        }
    }
    else {
        //parent
        signal(SIGINT,SIG_IGN);
        wait(NULL);
    }
}