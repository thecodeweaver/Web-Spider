/***
Project: Web Spider (C Version)
Version: 0.2
Filename: Crawler.c
Start Date: Aug. 4, 2019
About: Functions for crawling
***/

#include "Crawler.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// get_page_links
URL* get_page_links(char* page_content)
{
    // Start the URL list
    URL* url_list_head = calloc(1, sizeof(URL));
    if (url_list_head == NULL)
        return NULL;

    char* link_start = strstr(page_content, "<a");
    while (link_start != NULL)
    {
        // Find the link the <a tag
        char* href_start = strstr(link_start, "href=\"");
        if (href_start ==  NULL)
        {
            link_start = strstr(link_start, "<a");
            continue;
        }

        href_start += 6;

        // Find the end of the link
        char* link_end = strchr(href_start, '"');
        if (link_end == NULL)
        {
            link_start = strstr(href_start, "<a");
            continue;
        }

        // Copy the link
        int link_len = link_end - href_start;
        char* url = calloc(link_len + 1, sizeof(char));
        if (url == NULL)
        {
            link_start = strstr(link_end, "<a");
            continue;
        }

        strncpy(url, href_start, link_len);
        push_url(url_list_head, url);

        // Find the next URL
        link_start = strstr(link_end, "<a");
    }

    return url_list_head;
}
