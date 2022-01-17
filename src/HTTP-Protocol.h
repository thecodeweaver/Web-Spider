/***
Project: Web Spider (C Version)
Version: 0.2
Filename: HTTP-Protocol.h
Start Date: July 28, 2018
About: Header file for HTTP-Protocol.c
***/

#ifndef HTTP_PROTOCOL_H
#define HTTP_PROTOCOL_H

// http_get function
// Fetches a webpage using HTTP
// Arguments:
// url: the URL to request
// content: pointer where the request content will be stored (memory will be allocated)
// Returns the HTTP error code or -1 if some other error occurs
int http_get(char* url, char** content);

#endif //  #ifndef HTTP_PROTOCOL_H
