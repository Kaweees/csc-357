#include "safe_mem.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * A safe version of malloc that validates memory allocation and exits on
 failure
 * @param size the number of bytes to allocate in the heap
 * @return a pointer to successfully allocated memory
 */
void *safe_malloc(size_t size) {
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
void *safe_realloc(void *ptr, size_t size) {
  if (!(ptr = realloc(ptr, size))) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

/**
 * A safe version of free that validates memory allocation and exits on failure
 * @param ptr a pointer to the memory to free
 */
void safe_free(void *ptr) {
  if (ptr != NULL) {
    free(ptr);
    ptr = NULL;
  }
}

/**
 * A safe version of calloc that validates memory allocation and exits on
 failure
 * @param nmemb the number of elements to allocate
 * @param size the size of each element in bytes
 * @return a pointer to the successfully allocated memory
 */
void *safe_calloc(size_t nmemb, size_t size) {
  void *ptr;
  if (!(ptr = calloc(nmemb, size))) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }
  return ptr;
}