#include <arpa/inet.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "safe_dir.h"
#include "safe_file.h"
#include "safe_mem.h"

void mypwd() {
  struct stat* cwd_stat;
  struct stat* par_stat;
  safeLstat(".", cwd_stat);
  do {
    cwd_stat = safeMalloc(sizeof(struct stat));
    safeLstat(".", cwd_stat);
    safeChdir("..");
    par_stat = safeMalloc(sizeof(struct stat));
    safeLstat(".", par_stat);
  } while (!(cwd_stat->st_dev == par_stat->st_dev &&
             cwd_stat->st_ino == par_stat->st_ino));
  safeFree(cwd_stat);
  safeFree(par_stat);
}

// DIR* cwd = safeOpenDir(".");

// DirContent* dir_contents = safeReadDir(cwd);
// printf("%ld\n", dir_contents->num_entries);
// w

//     freeDirContent(dir_contents);

// char* cwd = getcwd(NULL, 0);
// if (cwd == NULL) {
//   perror("getcwd");
//   exit(EXIT_FAILURE);
// }
// printf("%s\n", cwd);
// free(cwd);

int main(int argc, char* argv[]) {
  mypwd();
  return EXIT_SUCCESS;
}