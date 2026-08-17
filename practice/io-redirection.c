#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/wait.h>
// runs 5-1.c >> test.txt
int main(int argc, char* argv[]) {
    int rc = fork();
    if (rc == 0) {
        //child process
        //redirecting standard output to a file
        close(STDOUT_FILENO);
        open("test.txt", O_CREAT|O_WRONLY|O_TRUNC);
        char* myargs[2];
        myargs[0] = strdup("5-1.c");
        myargs[1] = NULL;
        execvp(myargs[0],myargs); // runs 5-1.c
    }
    else {
        int wc = wait(NULL);
    }
    return 0 ;
}