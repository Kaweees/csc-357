#include "client.h"

#include <arpa/inet.h>
#include <ctype.h>
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

#include "talk.h"

#define MAX_USERNAME_SIZE    256  // Adjust the size as needed
#define RESPONSE_BUFFER_SIZE 256
#define LOCAL                0  // Refers to the local terminal
#define REMOTE               1  // Refers to the remote terminal
#define EXIT_MSG             "Connection closed.  ^C to terminate."
#define WAIT_TIME            150

extern volatile sig_atomic_t terminate;
extern void sigint_handler(int signo);

void clientMode(const char *hostname, int port, int verbose, int noNcurses) {
  /* File descriptors for the client socket */
  int client_socket;

  /* Create the client network socket */
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == CLIENT_FAILURE) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }

  /* Set up server address structure */
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);

  /* Convert the hostname to an IP address */
  if (inet_pton(AF_INET, hostname, &(server_address.sin_addr)) <= 0) {
    perror("Invalid address/ Address not supported");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  /* Connect to the server */
  if (connect(client_socket, (struct sockaddr *)&server_address,
          sizeof(server_address)) == CLIENT_FAILURE) {
    perror("Error connecting to server");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  /* Send the user's username to the server */
  char username[MAX_USERNAME_SIZE];
  snprintf(username, MAX_USERNAME_SIZE, "user@%s", hostname);
  send(client_socket, username, strlen(username), 0);

  /* Display message waiting for response */
  printf("Waiting for response from %s.\n", hostname);

  /* Receive response from the server */
  char response[RESPONSE_BUFFER_SIZE];
  recv(client_socket, response, RESPONSE_BUFFER_SIZE, 0);

  /* Check if the connection is accepted */
  if (strcmp(response, "ok") == 0) {
    printf("Connection accepted.\n");

    /* Start ncurses windowing if requested */
    if (!noNcurses) {
      start_windowing();
    }

    // Register the signal handler for SIGINT (Ctrl-C)
    signal(SIGINT, sigint_handler);

    char inputBuffer[MAX_BUFFER_SIZE];
    char remote_buffer[MAX_BUFFER_SIZE];

    struct pollfd fds[2];
    fds[LOCAL].fd = STDIN_FILENO;
    fds[LOCAL].events = POLLIN;
    fds[LOCAL].revents = 0;
    fds[REMOTE] = fds[LOCAL];
    fds[REMOTE].fd = client_socket;

    while (!terminate) {
      /* Poll for user input or server input */
      poll(fds, 2, -1);
      /* If the user has typed something, read it into the input buffer */
      if (fds[LOCAL].revents & POLLIN) {
        update_input_buffer();
        if (has_whole_line()) {
          int len = read_from_input(inputBuffer, MAX_BUFFER_SIZE);
          /* If EOF is hit, send the exit message and terminate */
          if (len == 0 && has_hit_eof()) {
            send(client_socket, EXIT_MSG, sizeof(EXIT_MSG) - 1, 0);
            if (!noNcurses) {
              stop_windowing();
            }
            close(client_socket);
            exit(EXIT_SUCCESS);
          } else if (has_hit_eof()) { /* If EOF is hit in the middle of a line,
                                       * send the line and terminate */
            inputBuffer[strlen(inputBuffer) - 1] = '\n';
            send(client_socket, inputBuffer, strlen(inputBuffer), 0);
            memset(inputBuffer, 0, sizeof(inputBuffer));
            send(client_socket, EXIT_MSG, sizeof(EXIT_MSG) - 1, 0);
            if (!noNcurses) {
              stop_windowing();
            }
            close(client_socket);
            exit(EXIT_SUCCESS);
          } else {
            /* Otherwise, send the line to the server */
            send(client_socket, inputBuffer, strlen(inputBuffer), 0);
            memset(inputBuffer, 0, sizeof(inputBuffer));
          }
        }
      }
      /* Handle printing output from the server */
      if (fds[REMOTE].revents & POLLIN) {
        int len = recv(fds[REMOTE].fd, remote_buffer, sizeof(inputBuffer), 0);
        if (len == 0) {
          /* If the server has closed the connection, print the exit message
           * and terminate */
          write_to_output(EXIT_MSG, sizeof(EXIT_MSG) - 1);
          close(client_socket);
          sleep(WAIT_TIME);
          if (!noNcurses) {
            stop_windowing();
          }
          return;
        } else {
          /* Otherwise, print the server's output */
          write_to_output(remote_buffer, len);
        }
        memset(remote_buffer, 0, sizeof(remote_buffer));
      }
    }
    /* Stop ncurses windowing */
    if (!noNcurses) {
      stop_windowing();
    }

    if (verbose) {
      printf("Connection terminated.\n");
    }

    /* Close the socket when done */
    close(client_socket);
  } else {
    /* If the connection is declined, print the message and terminate */
    printf("Connection declined.\n");
    /* Close the socket when done */
    close(client_socket);
  }
}
