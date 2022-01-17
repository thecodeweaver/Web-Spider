/***
Project: Web Spider (C Version)
Version: 0.2
Filename: Networking.c
Start Date: Aug. 1, 2019
About: Functions to make network programming easier/faster
***/

#include "Networking.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

// connect_to_host
int connect_to_host(char* host, char* port)
{
    // Resolve address
    struct addrinfo hints;
    struct addrinfo* info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(host, port, &hints, &info);

    if (err != 0)
    {
        #ifdef DEBUG
        printf("DEBUG: connect_to_host: getaddrinfo: %s\n", gai_strerror(err));
        #endif

        return -1;
    }

    // Create and connect socket
    int sockfd = 0;
    for (struct addrinfo* p = info; p != NULL; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockfd < 0)
        {
            // Try next address if we can't create a socket
            continue; 
        }

        err = connect(sockfd, p->ai_addr, p->ai_addrlen);

        if (err < 0)
        {
            close(sockfd);
            continue;
        } else
        {
            // We got a connection, cleanup and return
            freeaddrinfo(info);
            return sockfd;
        }
    }

    #ifdef DEBUG
    printf("DEBUG: connect_to_host: Unable to connect to %s:%s!\n", host, port);
    #endif

    return -1;
}