/***
Project: Web Spider (C Version)
Version: 0.2
Filename: URL.h
Start Date: July 29, 2019
About: Defines functions/types for working with URLs
***/

#ifndef URL_H
#define URL_H

// Type to represent a URL
struct URL
{
    char* url;
    char* url_host;
    char* url_protocol;
    char* url_path;
    struct URL* next_url; // Next URL in a linked list
};
typedef struct URL URL;

// MAKE_URL
// Macro to take a URL string, parses it and stores the result in a URL struct
#define MAKE_URL(url_struct, url_str) \
    url_struct.url = url_str; \
    url_struct.url_host = get_url_host(url_str); \
    url_struct.url_protocol = get_url_protocol(url_str); \
    url_struct.url_path = get_url_path(url_str); \
    url_struct.next_url = NULL;

// push_url
// Adds a URL to the end of a list of URLs
// Returns -1 if memory allocation fails
int push_url(URL* url_list, char* new_url);

// grab_url
// Gets a URL from a position in a list and returns a pointer to it
// Note: the first element is index 0
URL* grab_url(URL* url_list, int index);

// url_list_length
// Returns the length of a list of URLs
int url_list_length(URL* url_list);

// free_url_list
// Frees memory allocated to a URL list
void free_url_list(URL* url_list);

// get_url_host
// Gets the host part of a URL and returns it
// Returns NULL on error
// Note: Allocates memory
char* get_url_host(char* url);

// get_url_path
// Gets the path part of a URL and returns it
// Returns NULL on error
// Note: allocates memory
char* get_url_path(char* url);

// get_url_protocol
// Gets the protocol part of a URL and returns it
// Returns NULL on error
// Note: Allocates memory
char* get_url_protocol(char* url);

// Used for debugging
int dump_url_list(URL* list);

#endif
