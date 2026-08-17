#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    int p2=fork();
    if(p2==0){
        int p3=fork();
        if(p3==0){
            sleep(20);
            printf(1, "P3 : pid = %d, ppid = %d\n", getpid(), getppid());
            int p4=fork();
            if(p4==0){
                sleep(30);
                printf(1, "P4 : pid = %d, ppid = %d\n", getpid(), getppid());
                sleep(200);
                printf(1, "P4 : pid = %d, ppid = %d\n", getpid(), getppid());
                sleep(600);
                printf(1, "P4 : pid = %d, ppid = %d\n", getpid(), getppid());
                printf(1, "P4 exiting\n");
                exit();
            }
            else{
                int p5=fork();
                if(p5==0){
                    sleep(50);
                    printf(1, "P5 : pid = %d, ppid = %d\n", getpid(), getppid());
                    sleep(200);
                    printf(1, "P5 : pid = %d, ppid = %d\n", getpid(), getppid());
                    sleep(600);
                    printf(1, "P5 : pid = %d, ppid = %d\n", getpid(), getppid());
                    printf(1, "P5 exiting\n");
                    exit();
                }
                sleep(200);
                printf(1, "P3 exiting\n");
                exit();
            }
        }
        else{
            printf(1, "P2 : pid = %d, ppid = %d\n", getpid(), getppid());
            // sleep(50);
            int rp3 = wait();
            printf(1, "P2 reaped process PID %d\n", rp3);
            sleep(600);
            printf(1, "P2 exiting\n");
            exit();
        }
    }
    else{
        printf(1, "P1: pid = %d\n", getpid());
        int r1 = wait();
        printf(1, "P1 reaped process PID %d\n", r1);
        int r2 = wait();
        printf(1, "P1 reaped process PID %d\n", r2);
        int r3 = wait();
        printf(1, "P1 reaped process PID %d\n", r3);
    }
    exit();
}