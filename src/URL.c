/***
Project: Web Spider (C Version)
Version: 0.2
Filename: URL.c
Start Date: Aug. 2, 2019
About: Defines functions for working with URLs
***/

#include "URL.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// push_url
int push_url(URL* url_list, char* new_url)
{
    if (new_url == NULL || url_list == NULL)
    {
        return -1;
    }

    // Check if we have a list with only 1 node
    if (url_list->url == NULL)
    {
        url_list->url = calloc(strlen(new_url) + 1, sizeof(char));

        if (url_list->url == NULL)
        {
            return -1;
        }

        strncpy(url_list->url, new_url, strlen(new_url));

        url_list->next_url = NULL;
        return 0;
    }

    URL* last_url;
    for (URL* current_url = url_list; current_url != NULL; current_url = current_url->next_url)
    {
        last_url = current_url;
    }

    URL* new_url_struct = calloc(1, sizeof(URL));

    if (new_url_struct == NULL)
    {
        return -1;
    }

    new_url_struct->url = calloc(strlen(new_url) + 1, sizeof(char));

    if (new_url_struct->url == NULL)
    {
        free(new_url_struct);
        return -1;
    }

    strncpy(new_url_struct->url, new_url, strlen(new_url) + 1);

    // Lol I forgot to actually add the new struct to the list
    last_url->next_url = new_url_struct;

    return 0;
}

// grab_url
URL* grab_url(URL* url_list, int index)
{
    int current_index = 0;
    for (URL* current_url = url_list; current_url != NULL; current_url = current_url->next_url)
    {
        current_index++;
        if (current_index == index) { return current_url; }
    }
}

// url_list_length
int url_list_length(URL* url_list)
{
    int length = 1;
    for (URL* current_url = url_list; current_url != NULL; current_url = current_url->next_url)
    {
        length++;
    }

    return length;
}

// free_url_list
void free_url_list(URL* url_list)
{
    for (URL* current_url = url_list; current_url != NULL; current_url = current_url->next_url)
    {
        if (current_url->url != NULL)
            free(current_url->url);

        if (current_url->url_host != NULL)
            free(current_url->url_host);

        if (current_url->url_path != NULL)
            free(current_url->url_path);

        if (current_url->url_protocol != NULL)
            free(current_url->url_protocol);

        free(current_url);
    }
}

// Thank you HTTP Response Parser for these functions!

// get_url_host
char* get_url_host(char* url)
{
    // Find out where the host starts and ends in the URL
    char* host_start = strstr(url, "://");
    if (host_start == NULL) { return NULL; }
    host_start += 3; // +3 to pass the "://" characters

    char* host_end = strchr(host_start, '/');
    if (host_end == NULL) { return NULL; } 

    int host_length = host_end - host_start;

    // Copy the string
    char* host = calloc(host_length, sizeof(char));

    if (host == NULL) { return NULL; }

    strncpy(host, host_start, host_length);

    if (strlen(host) == 0)
    {
        free(host);
        return NULL;
    }

    return host;
}

// get_url_path
char* get_url_path(char* url)
{
    char* url_proto = get_url_protocol(url);
    if (url_proto == NULL) { return NULL; }
    
    char* url_host = get_url_host(url);
    if (url_host == NULL) { return NULL; }

    // Get a pointer to the start of the path using the length of the protocol, "://" and hostname
    // This is a nice little hack :D :face_with_sunglasses:
    char* path_start = url + strlen(url_proto) + strlen("://") + strlen(url_host);
    free(url_proto);
    free(url_host);

    char* path = calloc(strlen(path_start), sizeof(char));
    if (path == NULL) { return NULL; }

    strncpy(path, path_start, strlen(path_start));

    if (strlen(path) == 0)
    {
        free(path);
        return NULL;
    }

    return path;
}

// get_url_protocol
char* get_url_protocol(char* url)
{
    char* proto_end = strstr(url, "://");
    int proto_len = proto_end - url;

    char* proto = calloc(proto_len, sizeof(char));

    if (proto == NULL) { return NULL; }

    strncpy(proto, url, proto_len);

    if (strlen(proto) == 0)
    {
        free(proto);
        return NULL;
    }

    return proto;
}

int dump_url_list(URL* list)
{
    if (list == NULL)
        return -1;

    int count = 0;
    for (URL* i = list; i != NULL; i = i->next_url)
    {
        count++;
        printf("[%d] Address: %x\n", count, &i);
    }

    return 0;
}
