#include "safe_dir.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

DIR safeOpenDir(char *filename, int flags, mode_t mode) {
  DIR *dir;
  /* Open the directory */
  if ((dir = opendir(filename)) == NULL) {
    if (errno == ENOENT) {
      /* Directory does not exist. */
      perror("Directory does not exist.\n");
    } else if (errno == EACCES) {
      /* Insufficient permissions. */
      perror("Permission denied.\n");
    } else {
      /* Failed for some other reason */
      perror("Failed to open directory.\n");
    }
    exit(EXIT_FAILURE);
  } else {
    return dir;
  }
}

DirContent *safeReadDir(DIR *dir) {
  struct dirent *entry DirContent *dir_contents =
      safeMalloc(sizeof(DirContent));
  while ((entry = readdir(dir)) != NULL) {
    dir_contents->entries[dir_contents->num_entries] = entry;
    dir_contents->num_entries++;
  }
  return dir_contents;
}

void safeRewindDir(int fd) {}
void safeCloseDir(int fd) {}
void freeDirContent(DirContent *dir_contents) {}

// opendir(3) open a directory for reading
// readdir(3) read a directory entry
// rewinddir(3) rewind a directory to the beginning
// closedir(3) close a directory
// stat(2) get file status
// lstat(2) get file status