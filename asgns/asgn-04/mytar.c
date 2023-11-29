#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <math.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "tar.h"
#define CHAR_COUNT 256
/* parses command line arguments, only accepting valid
 * valid inputs. Then does the according process depending
 * on user input
 * */
int main(int argc, char *argv[]) {
  int checks[CHAR_COUNT] = {0}; /*used for parsing header*/
  int i;
  int vCheck, sCheck; /*verbosity and strict check */
  FILE *file;
  int valid = 0; /* for checking if input valid */
  if (argc < 3) {
    perror("usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
    return 1;
  }
  /*gets flags */
  for (i = 0; i < strlen(argv[1]); i++) {
    if (argv[1][i] != 'c' && argv[1][i] != 't' && argv[1][i] != 'S' &&
        argv[1][i] != 'v' && argv[1][i] != 'x' && argv[1][i] != 'f') {
      printf("unrecognized option '%c'.\n", argv[1][i]);
      valid = 1;
    }
    checks[(int)argv[1][i]] = 1;
  }
  /* user did not put valid option */
  if (valid == 1) {
    printf("you must choose one of the 'ctx' options.\n");
    fprintf(stderr, "usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
    exit(EXIT_FAILURE);
  }
  /* sets verbosity and strict flag */
  vCheck = checks['v'];
  sCheck = checks['S'];
  /* checks if input valid */
  if ((checks['f'] == 0) || (checks['c'] + checks['t'] + checks['x'] == 0) ||
      (checks['c'] + checks['t'] + checks['x'] > 1)) {
    fprintf(stderr, "usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
    exit(EXIT_FAILURE);
  }
  /* create option */
  if (checks['c'] == 1) {
    file = fopen(argv[2], "wb");
    for (i = 3; i < argc; i++) {
      traversePath(argv[i], file, vCheck, sCheck);
    }
    writeNullBlocks(file);
    fclose(file);
  } else if (checks['t'] == 1) /*list option */
  {
    listArchive(argc, argv, vCheck, sCheck);
  } else if (checks['x'] == 1) /*extract option */
  {
    extractArchive(argc, argv, vCheck, sCheck);
  }
  return 1;
}
