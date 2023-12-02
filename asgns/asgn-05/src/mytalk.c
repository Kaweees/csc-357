#include <ctype.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <talk.h>
#include <unistd.h>

#include "client.h"
#include "server.h"

#define OUT_OF_OPTIONS -1

#define USAGE_STRING(prog_name) \
  "Usage: mytalk [ -v ] [ -a ] [ -N ] [ hostname ] port\n"

/* Represents the options that can be passed to the program */
typedef enum TalkOptions {
  VERBOSE_OUTPUT = 'v',
  ACCEPT_CONNECTIONS = 'a',  // Server only
  NCURSES_MODE = 'N',
} TalkOptions;

/**
 * Prints the proper usage of the program and exits unsuccessfully.
 */
void usage() {
  fprintf(stderr, USAGE_STRING(argv[0]));
  exit(EXIT_FAILURE);
}

// Global flag to signal termination
volatile sig_atomic_t terminate = 0;

// Signal handler for Ctrl-C (SIGINT)
void sigint_handler(int signo) { terminate = 1; }

int main(int argc, char *argv[]) {
  enum TalkOptions opt;
  int verbose, accept_all, ncurses = 0;
  char *hostname = NULL;
  int port = 0;
  while ((opt = getopt(argc, argv, "vaN")) != OUT_OF_OPTIONS) {
    switch (opt) {
      case VERBOSE_OUTPUT:
        verbose = 1;
        break;
      case ACCEPT_CONNECTIONS:
        accept_all = 1;
        break;
      case NCURSES_MODE:
        ncurses = 1;
        break;
      default:
        usage();
    }
  }

  // Validate the remaining command line arguments
  if (optind < argc - 1) {
    hostname = argv[optind++];
  }

  if (optind != argc - 1) {
    usage();
  }

  port = atoi(argv[optind]);

  // Check if the program is in server or client mode
  if (hostname == NULL) {
    // Server mode
    serverMode(port, accept_all, verbose, ncurses);
  } else {
    // Client mode
    clientMode(hostname, port, verbose, ncurses);
  }
  return EXIT_SUCCESS;
}