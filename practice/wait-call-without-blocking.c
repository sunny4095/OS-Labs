#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(int arc, char* argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        //child process
        printf("Child process running ...\n");
        sleep(5);
        printf("Child process Exiting\n");
        return 0;
    }
    else if (pid > 0) {
        int status;
        while(1) {
            pid_t result = waitpid(pid,&status,WNOHANG);
            if (result == 0) {
                //child proccess is still running
                printf("Child is still running...\n");
                sleep(1);
            }
            else if (result == pid) {
                //child process has exited
                if (WIFEXITED(status)) {
                    printf("Child exited with status %d\n", WEXITSTATUS(status) );
                }
                else {printf("Child terminated abnormally \n");}
                break;
            }

        }
    }
}
