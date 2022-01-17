/***
Project: Web Spider (C Version)
Version: 0.2
Filename: HTTP-Protocol.c
Start Date: Aug. 2, 2019 (More than a year after starting HTTP-Protocol.h)
About: Very, very minimal implementation of the HTTP protocol
***/

#include "HTTP-Protocol.h"

#include "Networking.h"
#include "URL.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>


// http_get
int http_get(char* url, char** content)
{
    // Parse the URL
    URL url_struct;
    MAKE_URL(url_struct, url)

    // Check for URL parsing errors
    if (url_struct.url_host == NULL || url_struct.url_path == NULL || url_struct.url_protocol == NULL)
    {
        #ifdef DEBUG
        printf("DEBUG: http_get: Unable to parse URL %s!\n", url);
        #endif

        return -1;
    }

    // Build HTTP request
    char* http_request;
    int http_request_length = strlen("GET ") + strlen(url_struct.url_path) + strlen(" HTTP/1.1\r\n") + strlen("Host: ") + strlen(url_struct.url_host) + strlen("\r\nConnection: close\r\n\r\n") + 1;
    http_request = calloc(http_request_length, sizeof(char));
    if (http_request == NULL)
    {
        #ifdef DEBUG
        printf("DEBUG: http_get: Unable to allocate %d bytes for http_request!\n", http_request_length);
        #endif

        return -1;
    }

    snprintf(http_request, http_request_length, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", url_struct.url_path, url_struct.url_host);

    // Setup connection
    int sock_fd = connect_to_host(url_struct.url_host, "80");

    if (sock_fd < 0)
    {
        free(http_request);
        return -1;
    }

    // Send the HTTP request
    send(sock_fd, http_request, http_request_length - 1, 0);
    free(http_request);

    // Receive the entire HTTP response
    char* response_buffer;
    size_t response_length;
    FILE* response_stream = open_memstream(&response_buffer, &response_length);

    if (response_stream == NULL)
    {
        #ifdef DEBUG
        printf("DEBUG: http_get: open_memstream failed!\n");
        #endif

        return -1;
    }

    while (1)
    {
        char buffer[512];

        int bytes = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0)
        {
            break;
        }

        fwrite(buffer, sizeof(char), bytes, response_stream); 
    }

    close(sock_fd);
    fclose(response_stream);

    // Find the end of the HTTP headers and copy it into content
    char* headers_end = strstr(response_buffer, "\r\n\r\n");

    if (headers_end == NULL)
    {
        #ifdef DEBUG
        printf("DEBUG: http_get: Unable to find end of HTTP headers!\n");
        #endif

        free(response_buffer);
        return -1;
    }

    headers_end += 4; // Skip over the 'CRLFCRLF'

    *content = calloc(strlen(headers_end) + 1, sizeof(char));

    if (*content == NULL)
    {
        #ifdef DEBUG
        printf("DEBUG: http_get: Unable to allocate memory for page content!\n");
        #endif

        free(response_buffer);
        free(*content);
        return -1;
    }

    strncpy(*content, headers_end, strlen(headers_end));

    free(response_buffer);

    // We're finished :face_with_sunglasses:
    return 0;
}