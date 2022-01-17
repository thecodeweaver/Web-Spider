/***
Project: Web Spider (C Version)
Version: 0.2
Filename: String-Funcs.c
Start Date: April 22, 2018
About: Definitions of functions in string_funcs.h file
Test all functions
***/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "String-Funcs.h"

// combine_strings
char* combine_strings(char* string1, char* string2)
{
    int total_len = strlen(string1) + strlen(string2) + 1;

    char* tmp = realloc(string1, total_len);

    if (tmp == NULL)
    {
        return tmp;
    }

    strncat(tmp, string2, strlen(string2));

    return tmp;
}
