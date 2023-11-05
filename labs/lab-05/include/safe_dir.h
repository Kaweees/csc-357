#include <stdlib.h>
#include <unistd.h>

#include "limits.h"

#ifndef PATH_MAX
#define PATH_MAX
#define PATH_MAX 2048 /* Maximum number of characters in a path name */
#endif

#ifndef SAFE_DIR_H
#define SAFE_DIR_H

typedef struct DirContent DirContent;

/* Represents the contents of a directory */
struct DirContent {
  /* The number of entries in the directory */
  ssize_t num_entries;
  /* The pointer to the directory entries */
  struct dirent **entries;
};

int safeOpenDir(char *filename, int flags, mode_t mode);
DirContent *safeReadDir(int fd);
void safeRewindDir(int fd);
void safeCloseDir(int fd);
void freeDirContent(DirContent *dir_contents);

#endif