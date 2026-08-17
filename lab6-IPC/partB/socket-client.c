#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SOCK_PATH "a_socket"
int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_un server_addr;
    char buffer[16];

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_PATH);

    char* file_path = argv[argc-1];
    FILE* file = fopen(file_path,"r");
    int bytes_read = fread(buffer, 1, 16, file);
    while (bytes_read > 0) {
        int n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
        bytes_read = fread(buffer, 1, 16, file);
    }
    char end[2];
    end[0] = 'x';
    end[1] = 'x';
    int n = sendto(sockfd, end, strlen(end), 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
}