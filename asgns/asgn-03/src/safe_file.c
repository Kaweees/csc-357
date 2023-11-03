#include "safe_file.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "safe_mem.h"

#define FILE_ERROR -1

/**
 * A safe version of fopen that validates file opening and exits on failure
 * @param path the path to the file to open
 * @param mode the mode to open the file in
 * @return a file descriptor to the successfully opened file
 */
int safe_open(char *filename, int flags, mode_t mode) {
  int fd;
  if ((fd = open(filename, flags, mode)) == FILE_ERROR) {
    perror("Error opening file");
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
FileContent *safe_read(int fd) {
  struct stat file_info;
  if (fstat(fd, &file_info) == FILE_ERROR) {
    perror("Error getting file information");
    close(fd);
    exit(EXIT_FAILURE);
  } else {
    FileContent *file_content = (FileContent *)safe_malloc(sizeof(FileContent));
    file_content->file_size = file_info.st_size;
    printf("Poggies: %d\n", (int)file_info.st_size);
    file_content->file_contents = (char *)safe_malloc(file_content->file_size);
    if (read(fd, file_content->file_contents, file_content->file_size) ==
        FILE_ERROR) {
      perror("Error reading file");
      freeFileContent(file_content);
      close(fd);
      exit(EXIT_FAILURE);
    } else {
      return file_content;
    }
  }
}

/**
 * A safe version of fopen that validates file opening and exits on failure
 * @param path the path to the file to open
 * @param mode the mode to open the file in
 * @return a pointer to the successfully opened file
 */
void safe_file_write(int fd, void *buf, size_t count) {
  if (write(fd, buf, count) == FILE_ERROR) {
    perror("Error writing to file");
    exit(EXIT_FAILURE);
  }
}

/**
 * Frees the memory allocated for FileContent
 *
 * @param file_contents the FileContent to free
 */
void freeFileContent(FileContent *file_contents) {
  safe_free(file_contents->file_contents);
  free(file_contents);
}