#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROCESS_FAILURE 1

void forkit() {
  pid_t child;
  if ((child = fork()) == PROCESS_FAILURE) {
    perror("Error forking process");
    exit(EXIT_FAILURE);
  }
  if (child == 0) {
    printf("This is the child, pid %d\n", getpid());
    exit(EXIT_SUCCESS);
  } else {
    printf("This is the parent, pid %d\n", getpid());
  }
  if (wait(NULL) == PROCESS_FAILURE) {
    perror("Error waiting for child process");
    exit(EXIT_FAILURE);
  }
  printf("This is the parent, pid %d, signing off\n", getpid());
}

int main(int argc, char* argv[]) {
  /* Greet the world */
  printf("Hello, world!\n");
  forkit();
  return EXIT_SUCCESS;
}