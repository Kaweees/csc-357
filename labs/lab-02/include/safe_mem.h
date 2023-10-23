#include <stdlib.h>
#include <unistd.h>

void *safe_malloc(size_t size);
void *safe_realloc(void *ptr, size_t size);
void safe_free(void *ptr);
void *safe_calloc(size_t nmemb, size_t size);