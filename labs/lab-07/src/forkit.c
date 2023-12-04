#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define PROCESS_FAILURE 1
#define CHILD_PROCESS 0

void forkit() {
  pid_t cpid, w;
  int wstatus;
  if ((cpid = fork()) == PROCESS_FAILURE) {
    perror("Error forking process");
    exit(EXIT_FAILURE);
  }
  if (cpid == CHILD_PROCESS) { /* Code executed by child */
    printf("This is the child, pid %jd\n", (intmax_t) getpid());
    exit(EXIT_SUCCESS);
  } else { /* Code executed by parent */
    printf("This is the parent, pid %d\n", getpid());
  }
  w = waitpid(cpid, &wstatus, 0);
  if (w == PROCESS_FAILURE) {
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
  printf("This is the parent, pid %d, signing off\n", getpid());
}

int main(int argc, char* argv[]) {
  /* Greet the world */
  printf("Hello, world!\n");
  forkit();
  return EXIT_SUCCESS;
}

