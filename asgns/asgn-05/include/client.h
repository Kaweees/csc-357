#ifndef CLIENT_H
#define CLIENT_H

#define CLIENT_FAILURE   -1
#define LINE_BUFFER_SIZE 1024
#define MAX_BUFFER_SIZE  1024

void serverMode(int port, int acceptAll, int verbose, int ncurses);

#endif