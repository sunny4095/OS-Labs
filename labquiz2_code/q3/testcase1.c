#include "types.h"
#include "user.h"

int main(){
    int r = fork();
    if(r != 0){
        int r1 = fork();
        if(r1!=0){
            sleep(0);
            int r2 = fork();
            if(r2 != 0){
                wait();
            }
            else{
                get_sibling_info();
            }
            wait();
        }
        else{
            get_sibling_info();
            sleep(30);
        }
    }
    else{
        sleep(10);
        get_sibling_info();
        sleep(30);
    }
    wait();
    exit();

}