#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[]) {
    const char* path = "/usr/bin/ls";
    const char* argument = "-l";
    int x = execl(path, path, argument, NULL);
    // int x = execl("ls", NULL);
}