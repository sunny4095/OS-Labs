#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    int p2=fork();
    if(p2==0){
        int p3=fork();
        if(p3==0){
            printf(1, "P3 : pid = %d, ppid = %d\n", getpid(), getppid());
            sleep(100);
            printf(1, "P3 : pid = %d, ppid = %d\n", getpid(), getppid());
            exit();
        }
        else{
            printf(1, "P2 : pid = %d, ppid = %d\n", getpid(), getppid());
            sleep(10);
            printf(1, "P2 exiting\n");
            exit();
        }
    }
    else{
        printf(1, "P1: pid = %d\n", getpid());
        int r1 = wait();
        printf(1, "P1 reaped process PID %d\n", r1);
        sleep(50);
        int r2 = wait();
        printf(1, "P1 reaped process PID %d\n", r2);
    }
    exit();
}