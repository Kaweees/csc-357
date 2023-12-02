#include <stdio.h>

#ifndef SERVER_H
#define SERVER_H

#define SERVER_FAILURE   -1
#define LINE_BUFFER_SIZE 1024
#define MAX_BUFFER_SIZE  1024

void clientMode(const char *hostname, int port, int verbose, int ncurses);

#endif