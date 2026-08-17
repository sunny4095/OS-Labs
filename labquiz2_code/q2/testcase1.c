#include "types.h"
#include "user.h"

int main(){
    strace();
    sleep(1);
    sleep(1);
    write(1, "hello\n", 7);
    exit();
}