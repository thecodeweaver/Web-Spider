/***
Project: Web Spider (C Version)
Version: 0.2
Filename: string_funcs.h
Start Date: Mar. 26, 2018
About: Declarations of string handling functions
***/

#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

// combine_strings
// Combines 2 strings using realloc
// Returns NULL if memory allocation fails
// Note: string2 will be appended to string1
char* combine_strings(char* string1, char* string2);

#endif // End of #ifndef STRING_FUNCS_H