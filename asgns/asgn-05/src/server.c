#include "server.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "talk.h"

// Global flag to signal termination
volatile sig_atomic_t terminate = 0;

// Signal handler for Ctrl-C (SIGINT)
void sigint_handler(int signo) { terminate = 1; }

void serverMode(int port, int acceptAll, int verbose, int noNcurses) {
  /* File descriptors for the server and client sockets */
  int server_socket, client_socket;

  /* Create the server network socket */
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == SERVER_FAILURE) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }

  /* Set up server address structure */
  struct sockaddr_in server_address, client_address;
  socklen_t client_length = (socklen_t)sizeof(client_address);
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(port);

  /* Bind the server socket to the specified address */
  if (bind(server_socket, (struct sockaddr *)&server_address,
          sizeof(server_address)) == SERVER_FAILURE) {
    perror("Error binding socket");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  if (verbose) {
    printf("Server listening at %s:%d\n", inet_ntoa(server_address.sin_addr),
        port);
  }

  /* Wait and listen for incoming connections */
  if (listen(server_socket, 5) == SERVER_FAILURE) {
    perror("Error listening for connections");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  /* Accept incoming connections */
  client_socket =
      accept(server_socket, (struct sockaddr *)&client_address, &client_length);
  if (client_socket == SERVER_FAILURE) {
    perror("Error accepting connection");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  /* Display message requesting acceptance */
  printf("Mytalk request from user@%s. Accept (y/n)? ",
      inet_ntoa(client_address.sin_addr));
  char line[LINE_BUFFER_SIZE];
  fgets(line, LINE_BUFFER_SIZE, stdin);
  size_t i = 0;
  for (i = 0; i < strlen(line); i++) {
    line[i] = tolower(line[i]);
  }
  if (acceptAll && strcmp(line, "y") == 0 || strcmp(line, "yes") == 0) {
    // Accept the connection
    printf("Connection accepted.\n");
    send(client_socket, "ok", 2, 0);

    /* Start the ncurses windows if requested */
    if (!noNcurses) {
      start_windowing();
    }

    char inputBuffer[MAX_BUFFER_SIZE];
    char remote_buffer[MAX_BUFFER_SIZE];

    while (!terminate) {
      /* Send user input to remote user */
      if (has_whole_line()) {
        update_input_buffer();
        read_from_input(inputBuffer, MAX_BUFFER_SIZE);
        write_to_output(inputBuffer, strlen(inputBuffer));
      }

      /* Check for remote user input */
      if (recv(client_socket, remote_buffer, MAX_BUFFER_SIZE, MSG_DONTWAIT) >
          0) {
        write_to_output(remote_buffer, strlen(remote_buffer));
      }

      /* Check for termination (EOF or SIGINT) */
      if (has_hit_eof()) {
        terminate = 1;
      }
    }

    /* Stop ncurses windowing */
    if (!noNcurses) {
      stop_windowing();
    }
  } else {
    // Decline the connection
    send(client_socket, "no", 2, 0);
    printf("Connection declined.\n");
  }
  /* Close the sockets when done */
  close(client_socket);
  close(server_socket);
}