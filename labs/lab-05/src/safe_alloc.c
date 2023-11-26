/*
 * safe_alloc.c - wrapper for allocator sycalls with an emphasis on safety and
 simplicity
 *
 * All of the memory allocation-related syscalls are wrapped in a way that
 emphasizes safety and simplicity. The results of every syscall are validated
 for intended behavior upon execution.
 */
#include "safe_alloc.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * A safe version of malloc that validates memory allocation and exits on
 failure
 * @param size the number of bytes to allocate in the heap
 * @return a pointer to successfully allocated memory
 */
void *safeMalloc(size_t size) {
  void *ptr;
  if (!(ptr = malloc(size))) {
    fprintf(stderr, "Memory allocation error.\n");
    perror("mypwd");
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
    fprintf(stderr, "Memory allocation error.\n");
    perror("mypwd");
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
    fprintf(stderr, "Memory allocation error.\n");
    perror("mypwd");
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