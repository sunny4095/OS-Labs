#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2]; 
    char buffer[256];

    //create a pipe
    pipe(pipefd);

    int cpid = fork();

    if (cpid == 0) {
        //child process
        close(pipefd[1]); // close write end
        read(pipefd[0],buffer,sizeof(buffer));
        printf("Message child recieved : %s\n",buffer);
        close(pipefd[0]);
    }

    if (cpid > 0) {
        //parent process
        close(pipefd[0]); //close read end
        char* message = "I am sunny";
        write(pipefd[1],message,strlen(message)+1);
        close(pipefd[1]);
    }
}