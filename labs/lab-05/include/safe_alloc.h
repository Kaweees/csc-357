#include <stdlib.h>
#include <unistd.h>

#ifndef SAFE_ALLOC_H
#define SAFE_ALLOC_H

void *safeMalloc(size_t size);
void *safeRealloc(void *ptr, size_t size);
void *safeCalloc(size_t nmemb, size_t size);
void safeFree(void *ptr);

#endif /* SAFE_ALLOC_H */