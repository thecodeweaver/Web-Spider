/***
Project: Web Spider (C Version)
Version: 0.2
Filename: Main.c
Start Date: Aug. 4, 2019
About: After over a year, finally writing the main function for Web Spider!
TODO: Figure out get_page_links();
***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Crawler.h"
#include "HTTP-Protocol.h"
#include "URL.h"

#define WEB_SPIDER_VERSION 0.2

// Commandline usage function
// Not sure why I put this in Main.c
void usage(char* args[]);

int main(int argc, char* argv[])
{
    // Get command line arguments
    if (argc != 4)
    {
        usage(argv);
    }

    char* search_term = argv[1];
    char* start_url = argv[2];
    int search_results_count = atoi(argv[3]);

    // Get the time when crawling was started
    time_t start_time = time(NULL);

    #ifdef DEBUG
    printf("DEBUG: Fetching start URL %s...\n", start_url);
    #endif

    // Fetch the start URL
    char* current_content;
    int http_status_code = http_get(start_url, &current_content);

    if (http_status_code != 0)
    {
        fprintf(stderr, "ERROR: Unable to fetch start URL %s!\n", start_url);
        exit(-1);
    }

    #ifdef DEBUG
    printf("DEBUG: Received %d bytes from %s.\n", (int) strlen(current_content), start_url);
    printf("DEBUG: Extracting links from the start URL...\n");
    #endif

    // Get links from start URL
    URL* links_to_visit = get_page_links(current_content);

    if (links_to_visit == NULL)
    {
        fprintf(stderr, "ERROR: Unable to get links from start URL %s!\n", start_url);
        exit(-1);
    }

    #ifdef DEBUG
    printf("DEBUG: Extracted %d links.\n", url_list_length(links_to_visit));
    #endif

    free(current_content);

    #ifdef DEBUG
    printf("DEBUG: Starting the main crawler loop...\n");
    #endif

    URL* search_results_list = calloc(1, sizeof(URL));
    if (search_results_list == NULL)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(-1);
    }

    // Main loop
    int search_results = 0;
    int links_visited = 1;
    while (search_results != search_results_count)
    {
        // Make sure there are still links to visit
        if (url_list_length(links_to_visit) == 0)
        {
            break;
        }

        // Pay the next URL a visit
        if (links_to_visit->url == NULL)
        {
            // Remove the current URL
            URL* temp_url = links_to_visit->next_url; // Save the rest of the list
            free(links_to_visit->url);
            free(links_to_visit);
            links_to_visit = temp_url; // Connect back to the rest of the list
            continue;
        }

        #ifdef DEBUG
        printf("DEBUG: Fetching URL %s...\n", links_to_visit->url);
        #endif

        http_status_code = http_get(links_to_visit->url, &current_content);

        // Move on to the next URL if there is an error
        if (http_status_code == -1)
        {
            #ifdef DEBUG
            printf("DEBUG: Fetching URL %s failed!\n", links_to_visit->url);
            #endif

            // Remove the current URL
            URL* temp_url = links_to_visit->next_url; // Save the rest of the list
            free(links_to_visit->url);
            free(links_to_visit);
            links_to_visit = temp_url; // Connect back to the rest of the list
            continue;
        }

        links_visited++;

        if (strlen(current_content) == 0)
        {
            // Remove the current URL
            free(current_content); // Not sure if this will cause a double free issue
            URL* temp_url = links_to_visit->next_url; // Save the rest of the list
            free(links_to_visit->url);
            free(links_to_visit);
            links_to_visit = temp_url; // Connect back to the rest of the list
            continue;
        }

        #ifdef DEBUG
        printf("DEBUG: Received %d bytes from URL %s.\n", (int) strlen(current_content), links_to_visit->url);
        printf("DEBUG: Extracting links from URL %s...\n", links_to_visit->url);
        #endif

        // Get links from the current page
        URL* current_page_links = get_page_links(current_content);

        if (current_page_links != NULL)
        {
            for (URL* p = current_page_links; p != NULL; p = p->next_url)
            {
                push_url(links_to_visit, p->url);
            }

            free_url_list(current_page_links);
        }

        #ifdef DEBUG
        printf("DEBUG: Extracted %d links.\n", url_list_length(current_page_links));
        #endif

        // Check the content for the search term
        if (strstr(current_content, search_term) != NULL)
        {
            #ifdef DEBUG
            printf("DEBUG: Search term \'%s\' found!\n", search_term);
            #endif

            search_results++;
            push_url(search_results_list, links_to_visit->url);
        }

        free(current_content); // Not sure if this will actually do anything...

        // Remove the current URL
        URL* temp_url = links_to_visit->next_url; // Save the rest of the list
        free(links_to_visit->url);
        free(links_to_visit);
        links_to_visit = temp_url; // Connect back to the rest of the list

        printf("%d links to go.\n", url_list_length(links_to_visit));
    }

    // TODO: Get end time and print the complete crawling time

    // Check the number of search results
    if (search_results_count != search_results)
    {
        fprintf(stderr, "Error: Unable to find search term %s!\n", search_term);
        free_url_list(links_to_visit);
        free_url_list(search_results_list);
        return -1;
    } else
    {
        // Print out the search results
        printf("Found %d search results!\n", url_list_length(search_results_list));
        printf("Search Results: \n");

        #ifdef DEBUG
        if (dump_url_list(search_results_list) < 0)
        {
            printf("DEBUG: Unable to read search_results_list!\n");
            free_url_list(links_to_visit);
            free_url_list(search_results_list);
            return -1;
        }
        #endif

        /*
        // TODO: Program segfaults here, check for invalid memory access
        for (URL* i = search_results_list; i != NULL; i = i->next_url)
        {
            printf("%s\n", i->url);
        }
        */

        free_url_list(search_results_list);
    }

    // And the crawler is finally finished after over a YEAR!
    return 0;
}

// usage
void usage(char* args[])
{
    printf("\t!===== Web Spider! =====!\n");
    printf("Version %.1f\n", WEB_SPIDER_VERSION);
    printf("Usage: %s [search term] [start url] [number of search results]\n", args[0]);
    exit(0);
}
