#include "safe_file.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "safe_mem.h"

/**
 * A safe version of fopen that validates file opening and exits on failure
 * @param path the path to the file to open
 * @param mode the mode to open the file in
 * @return a file descriptor to the successfully opened file
 */
int safeOpen(char *filename, int flags, mode_t mode) {
  int fd;
  if ((fd = open(filename, flags, mode)) == FILE_ERROR) {
    perror("Error opening file.\n");
    exit(EXIT_FAILURE);
  } else {
    return fd;
  }
}

/**
 * Opens a file and stores the contents in a FileContent
 *
 * @param fd the file pointer to read from
 */
FileContent *safeRead(int fd) {
  struct stat file_info;
  if (fstat(fd, &file_info) == FILE_ERROR) {
    perror("Error getting file information.\n");
    close(fd);
    exit(EXIT_FAILURE);
  } else {
    FileContent *file_content =
        (FileContent *)safeCalloc(sizeof(FileContent), 1);
    file_content->file_size = file_info.st_size;
    file_content->file_contents = (unsigned char *)safeCalloc(
        sizeof(unsigned char), file_content->file_size);
    if (read(fd, file_content->file_contents, file_content->file_size) ==
        FILE_ERROR) {
      perror("Error reading file.\n");
      freeFileContent(file_content);
      close(fd);
      exit(EXIT_FAILURE);
    } else {
      return file_content;
    }
  }
}

/**
 * A safe version of write that validates writing to files and exits on failure
 * @param path the path to the file to open
 * @param mode the mode to open the file in
 * @return a pointer to the successfully opened file
 */
void safeWrite(int fd, void *buf, size_t count) {
  if (write(fd, buf, count) == FILE_ERROR) {
    perror("Error writing to file.\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * A safe version of close that validates file closing and exits on failure
 * @param stream the file pointer to close
 */
void safeClose(int fd) {
  if (close(fd) == FILE_ERROR) {
    perror("Error closing file.\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * Frees the memory allocated for FileContent
 *
 * @param file_contents the FileContent to free
 */
void freeFileContent(FileContent *file_contents) {
  safeFree(file_contents->file_contents);
  free(file_contents);
}