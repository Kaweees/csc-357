#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process_values.h"

#define MIN_NUM_ARGS   2 /* The minimum number of command-line arguments*/
#define PROGRAM_NAME   0 /* The index of the program name in argv */
#define PROGRAM_LOADED 1 /* The index of the program being executed */
#define USAGE_STRING \
  "Usage: %s command [arg1 arg2 ...]\n" /* The program usage string */

/**
 * Prints the proper usage of the program and exits unsuccessfully.
 * @param argv - the command-line arguments
 */
void usage(char* program_name) {
  fprintf(stderr, USAGE_STRING, program_name);
  exit(EXIT_FAILURE);
}

/**
 * Forks a child process and executes the given command in the child process.
 * @param command - the command to execute
 * @param args - the arguments to the command
 */
void tryit(char* command, char* args[]) {
  pid_t cpid, w;
  int wstatus;
  if ((cpid = fork()) == PROCESS_FAILURE) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (cpid == CHILD_PROCESS) { /* Code executed by child */
    exit(EXIT_SUCCESS);
  } else { /* Code executed by parent */
    execvp(command, args);
  }
  if ((w = waitpid(cpid, &wstatus, ZERO_OPTIONS)) == PROCESS_FAILURE) {
    perror("waitpid");
    exit(EXIT_FAILURE);
  }
  if (WIFEXITED(w)) {
    printf("exited, status=%d\n", WEXITSTATUS(w));
  } else if (WIFSIGNALED(w)) {
    printf("killed by signal %d\n", WTERMSIG(w));
  } else if (WIFSTOPPED(w)) {
    printf("stopped by signal %d\n", WSTOPSIG(w));
  } else if (WIFCONTINUED(w)) {
    printf("continued\n");
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