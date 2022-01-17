#!/bin/bash
# Script to build Web Spider with debug options and run a basic test case
# Oct. 20, 2019

rm -rf spider
gcc -g -DDEBUG -o spider src/*.c
./spider 787 http://www.boeing.com/ 3
