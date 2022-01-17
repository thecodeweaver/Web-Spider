/***
Project: Web Spider (C Version)
Version: 0.2
Filename: Networking.h
Start Date: July 29, 2019
About: Functions to make network programming easier/faster
***/

#ifndef NETWORKING_H
#define NETWORKING_H

#include <netinet/in.h>

// BUILD_SOCKADDR_IN
// Macro to build a sockaddr_in struct
#define BUILD_SOCKADDR_IN(address, ip, port, address_family) \
    address.sin_family = address_family; \
    address.sin_port = htons(port); \
    inet_pton(address_family, ip, &(address.sin_addr));

// connect_to_host
// Creates a socket, resolves the host name and connects the socket
// Basically a front end of getaddrinfo and connect
// Returns -1 on error, otherwise returns the new socket
int connect_to_host(char* host, char* port);

#endif