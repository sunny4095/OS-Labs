#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SOCK_PATH "unix_socket"

int main() {
    int sockfd;
    char buffer[256];
    struct sockaddr_un serv_addr, cli_addr;

    //create socket
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

    //fill in the socket address
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);

    //bind socket to this address
    bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    //read message from client
    socklen_t n = sizeof(cli_addr);
    int bytes_recieved = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, &n);
    printf("The message is :\n%s\n",buffer);

    unlink(SOCK_PATH);

}