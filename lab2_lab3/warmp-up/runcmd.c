#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Incorrect number of argumnets\n");
        exit(1);
    }
    const char* command = argv[1];
    const char* argument = argv[2];
    int cpid = fork();
    if (cpid == 0) {
        int x = execlp(command,command,argument,NULL);
    }
    if (cpid > 0) {
        int x = wait(NULL);
        printf("Command successfully complated\n");
    }
}