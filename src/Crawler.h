/***
Project: Web Spider (C Version)
Version: 0.2
Filename: Crawler.h
Start Date: Aug. 4, 2019
About: Header file for crawling functions
***/

#ifndef CRAWLER_H
#define CRAWLER_H

#include "URL.h"

// get_page_links
// Returns a list of all the URLs on a web page
// Returns NULL if no links are found or if an error occurs
URL* get_page_links(char* page_content);

#endif