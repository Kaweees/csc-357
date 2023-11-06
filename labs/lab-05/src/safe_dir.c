#include "safe_dir.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * A safe version of opendir that validates the directory stream and exits on failure
 *
 * @param filename The name of the directory to open.
 * @param flags The flags to use when opening the directory.
 * @param mode The mode to use when opening the directory.
 * @return A pointer to the directory stream.
 */
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

/**
 * A safe version of readdir that validates the directory contents and exits on failure
 *
 * @param dir The directory stream to read from.
 * @return A pointer to the directory contents.
 */
DirContent *safeReadDir(DIR *dir) {
  struct dirent *entry DirContent *dir_contents =
      safeMalloc(sizeof(DirContent));
  while ((entry = readdir(dir)) != NULL) {
    dir_contents->entries[dir_contents->num_entries] = entry;
    dir_contents->num_entries++;
  }
  return dir_contents;
}

/**
 * A safe version of rewinddir that validates the rewinded stream and exits on failure
 *
 * @param fd The directory stream to rewind.
 */
void safeRewindDir(int fd) {}

/**
 * A safe version of closedir that validates the closed stream and exits on failure
 *
 * @param fd The directory stream to rewind.
 */
void safeCloseDir(int fd) {}


/**
 * A safe version of stat that validates the file status (symlinks are followed) and exits on failure
 *
 * @param path The path to the file to stat.
  * @param buf The buffer to store the file status in.
 */
void safeStat(char *path, struct stat *buf) {
  if (stat(path, buf) == -1) {
    perror("Failed to stat file");
    exit(EXIT_FAILURE);
  }
}

/**
 * A safe version of lsat that validates the file status (symlinks aren't followed) and exits on failure
 *
 * @param path The path to the file to stat.
 * @param buf The buffer to store the file status in.
 */
void safeLstat(char *path, struct stat *buf) {
  if (lstat(path, buf) == -1) {
    perror("Failed to stat file");
    exit(EXIT_FAILURE);
  }
}
