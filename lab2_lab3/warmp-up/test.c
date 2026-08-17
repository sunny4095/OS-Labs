#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

void signal_handler() { printf("Signal received\n"); exit(0); }

int main() {
    int N = 3; int i = 0;
    while(i<N) {
    int cpid = fork(); i++;
    if(cpid == 0) {
        printf("child i=%d\n", i);
        signal(SIGINT, signal_handler);
        while(1);
    }
    else {
        sleep(1); kill(cpid, SIGINT); wait(NULL);
    }
    }
}