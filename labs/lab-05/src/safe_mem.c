#include "safe_mem.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define FILE_ERROR -1

struct stat file_info;

/**
 * A safe version of malloc that validates memory allocation and exits on
 failure
 * @param size the number of bytes to allocate in the heap
 * @return a pointer to successfully allocated memory
 */
void *safeMalloc(size_t size) {
  void *ptr;
  if (!(ptr = malloc(size))) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

/**
 * A safe version of realloc that validates memory allocation and exits on
 failure
 * @param ptr a pointer to the memory to reallocate
 * @param size the number of bytes to allocate in the heap
 * @return a pointer to the successfully reallocated memory
 */
void *safeRealloc(void *ptr, size_t size) {
  if (!(ptr = realloc(ptr, size))) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

/**
 * A safe version of calloc that validates memory allocation and exits on
 failure
 * @param nmemb the number of elements to allocate
 * @param size the size of each element in bytes
 * @return a pointer to the successfully allocated memory
 */
void *safeCalloc(size_t nmemb, size_t size) {
  void *ptr;
  if (!(ptr = calloc(nmemb, size))) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

/**
 * A safe version of free that validates memory allocation and exits on failure
 * @param ptr a pointer to the memory to free
 */
void safeFree(void *ptr) {
  if (ptr != NULL) {
    free(ptr);
    ptr = NULL;
  }
}