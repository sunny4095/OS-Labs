#include "types.h"
#include "user.h"

int main(){
    sleep(1);
    sleep(1);
    int r = fork();
    if(r == 0){
        strace();
    }
    else{
        wait();
    }
    printf(1, "out\n");
    exit();
}