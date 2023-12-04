#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MIN_NUM_ARGS 2 /* The minimum number of command-line arguments*/
#define PROCESS_FAILURE -1 /* The process failure return value*/
#define CHILD_PROCESS 0 /* The child process return value*/
#define PROGRAM_NAME 0 /* The index of the program name in argv */
#define PROGRAM_LOADED 1 /* The index of the program being executed */

#define USAGE_STRING(prog_name) \
  "Usage: " #prog_name "command [arg1 arg2 ...]\n" \

/**
 * Prints the proper usage of the program and exits unsuccessfully.
 * @param argv - the command-line arguments
 */
void usage(char* program_name) {
  fprintf(stderr, USAGE_STRING(%s), program_name);
  exit(EXIT_FAILURE);
}

/**
 * Forks a child process and executes the given command in the child process.
 * @param command - the command to execute
 * @param args - the arguments to the command
 */
void tryit(char* command, char* args[]) {
  for (int i = 0; args[i] != NULL; i++) {
    printf("%s\n", args[i]);
  }
  pid_t child;
  if ((child = fork()) == PROCESS_FAILURE) {
    perror("Error forking process");
    exit(EXIT_FAILURE);
  }
  if (child == CHILD_PROCESS) {
    execvp(command, args);
    perror("Error executing command");
    exit(EXIT_FAILURE);
  } else {
    if (wait(NULL) == PROCESS_FAILURE) {
      perror("Error waiting for child process");
      exit(EXIT_FAILURE);
    }
  }
}

/**
 * Program entry point.
 * @param argc - the number of command-line arguments
 * @param argv - the command-line arguments
 * @return EXIT_SUCCESS if the program runs successfully, EXIT_FAILURE otherwise
 */
int main(int argc, char* argv[]) {
  if (argc < MIN_NUM_ARGS) {
    usage(argv[PROGRAM_NAME]);
  } else {
    tryit(argv[PROGRAM_LOADED], argv + PROGRAM_LOADED);
  }
  return EXIT_SUCCESS;
}