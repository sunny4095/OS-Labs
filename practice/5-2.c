#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hallo (pid:%d)\n,", (int) getpid());
    int rc=fork();
    if (rc<0) {
        // fork faied
        fprintf(stderr, "fork failed");
    } else if (rc==0) {
        // child process enters here
        printf("child (pid:%d)", (int) getpid());
        
    }
    
}
