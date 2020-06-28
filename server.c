#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "message.h"

int main(int argc, char **argv) {

    struct sockaddr_storage their_addr;
    struct addrinfo hints, *res, *p;
    int udpsocket;
    int numbytes;


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; //AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "8888", &hints, &res);

    for (p = res; p != NULL; p = p->ai_next) {
        if ((udpsocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("socket err\n");
            continue;
        }

        if (bind(udpsocket, p->ai_addr, p->ai_addrlen) == -1) {
            close(udpsocket);
            printf("bind err\n");
            continue;
        }

        break;
    }

    if (p == NULL) {
        printf("Failed to bind socket\n");
        return EXIT_FAILURE;
    }

    freeaddrinfo(res);

    printf("waiting to recvfrom...\n");

    char buf[512];
    memset(buf, 0, sizeof(buf));
    socklen_t addr_len = sizeof(their_addr);
    if ((numbytes = recvfrom(udpsocket, buf, 511, 0, 
                             (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        printf("ERROR recvfrom\n");
        return EXIT_FAILURE;
    }

    printf("packet is %d bytes long\n", numbytes);
    buf[512] = '\0';

    // TODO - let's go through the data and parse out all of the fields
    printf("data is:\n");
    for (int i = 0; i < numbytes; i++) {
        printf("%x ", buf[i] & 0xFF);
    }

    parse_message(buf, numbytes);

    // clean up
    close(udpsocket);

    return EXIT_SUCCESS;
}
