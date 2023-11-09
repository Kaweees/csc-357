#include <stdlib.h>
#include <unistd.h>
#include <bits/types.h>
#include <time.h>

#ifndef FILE_ERROR
#define FILE_ERROR -1
#endif

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

int safeOpen(char *filename, int flags, mode_t mode);
FileContent *safeRead(int fd);
void safeWrite(int fd, void *buf, size_t count);
void safeClose(int fd);
void freeFileContent(FileContent *file_contents);
#endif