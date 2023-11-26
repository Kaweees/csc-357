#include <stdlib.h>
#include <unistd.h>

#ifndef SAFE_FILE_H
#define SAFE_FILE_H

typedef struct FileContent FileContent;

/* Represents the contents of a file */
struct FileContent {
  /* The length of the file contents in bytes */
  ssize_t file_size;
  /* The pointer to the file contents */
  unsigned char *file_contents;
};

int safe_open(char *filename, int flags, mode_t mode);
FileContent *safe_read(int fd);
void safe_write(int fd, void *buf, size_t count);
void freeFileContent(FileContent *file_contents);

#endif