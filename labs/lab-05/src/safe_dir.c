#include "safe_dir.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "safe_mem.h"

/**
 * A safe version of opendir that validates the directory stream and exits on
 * failure
 *
 * @param path The path to the directory to open.
 * @return A pointer to the directory stream.
 */
DIR *safeOpenDir(const char *path) {
  DIR *dir;
  /* Open the directory */
  if ((dir = opendir(path)) == NULL) {
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
 * A safe version of readdir that validates the directory contents and exits on
 * failure
 *
 * @param dir The directory stream to read from.
 * @return A pointer to the directory contents.
 */
DirContent *safeReadDir(DIR *dir) {
  DirContent *dir_contents = safeMalloc(sizeof(DirContent));
  dir_contents->entries = NULL;
  dir_contents->num_entries = 0;
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    /* Skip hidden files */
    if (entry->d_name[0] != '.') {
      dir_contents->entries =
          (struct dirent **)safeRealloc(dir_contents->entries,
              (dir_contents->num_entries + 1) * sizeof(struct dirent *));

      dir_contents->entries[dir_contents->num_entries] = entry;
      dir_contents->num_entries++;
    }
  }
  return dir_contents;
}

/**
 * A safe version of rewinddir that validates the rewinded stream and exits on
 * failure
 *
 * @param dir The directory stream to rewind.
 */
void safeRewindDir(DIR *dir) { rewinddir(dir); }

/**
 * A safe version of closedir that validates the closed stream and exits on
 * failure
 *
 * @param fd The directory stream to rewind.
 */
void safeCloseDir(DIR *dir) { closedir(dir); }

/**
 * A safe version of stat that validates the file status (symlinks are followed)
 * and exits on failure
 *
 * @param path The path to the file to stat.
 * @param buf The buffer to store the file status in.
 */
void safeStat(char *path, struct stat *buf) {
  if (stat(path, buf) == DIR_ERROR) {
    perror("Failed to stat file.\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * A safe version of lsat that validates the file status (symlinks aren't
 * followed) and exits on failure
 *
 * @param path The path to the file to stat.
 * @param buf The buffer to store the file status in.
 */
void safeLstat(char *path, struct stat *buf) {
  if (lstat(path, buf) == DIR_ERROR) {
    perror("Failed to stat file.\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * A safe version of chdir that validates the changed directory and exits on
 * failure
 *
 * @param path The path to the directory to change to.
 */
void safeChdir(char *path) {
  if (chdir(path) == DIR_ERROR) {
    perror("Failed to change directory.\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * Frees the memory allocated for DirContent
 *
 * @param dir_contents The DirContent to free.
 */
void freeDirContent(DirContent *dir_contents) {
  safeFree(dir_contents->entries);
  safeFree(dir_contents);
}