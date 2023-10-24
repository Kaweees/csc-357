#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "safe_mem.h"

#define INITIAL_BUFFER_SIZE 128  /* Initial size of the buffer in bytes */
#define NEWLINE_CHAR        10   /* asci code for a newline */
#define CARRIAGE_CHAR       13   /* asci code for a carriage return */
#define STRING_TERMINATOR   '\0' /* null terminator for a string */

/* Represents an arbitrarily long line */
struct LongLine {
  /* The length of the line in bytes */
  size_t capacity;
  /* The pointer to the line */
  char *line;
};

/**
 * Reads an arbitrarily long line from the given file into newly-allocated
 * space
 *
 * @param file - a pointer to the file to read the line from
 * @return a pointer to a LongLine struct containing the line and its length
 */
struct LongLine *read_long_line(FILE *file) {
  /* line read from buffer */
  struct LongLine *long_line =
      (struct LongLine *)safe_malloc(sizeof(struct LongLine));
  long_line->capacity = INITIAL_BUFFER_SIZE;
  /* Allocate valid space for the line read from buffer */
  long_line->line = (char *)safe_calloc(long_line->capacity, sizeof(char));
  size_t buffer_size = 0;
  int ch; /* character read from file */

  /* Read characters from the file until end of line or newline read */
  while (
      (ch = fgetc(file)) != EOF && ch != NEWLINE_CHAR && ch != CARRIAGE_CHAR) {
    /* Reallocate space if buffer is full, minus two because of the null
     * character */
    if (buffer_size >= long_line->capacity - 1) {
      long_line->capacity *= 2;  // Double the capacity
      long_line->line =
          (char *)safe_realloc(long_line->line, long_line->capacity);
    }
    /* Add the character to the line and increment line_length after */
    long_line->line[buffer_size++] = (char)ch;
  }

  if (ch == EOF || long_line->capacity == 0) {
    free(long_line->line);
    free(long_line);
    return NULL;
  } else {
    long_line->line[buffer_size] = NEWLINE_CHAR;
    return long_line;
  }
}

void uniq(FILE *fp) {
  struct LongLine *prev_line = NULL;
  struct LongLine *current_line = NULL;

  while ((current_line = read_long_line(fp)) != NULL) {
    if (prev_line == NULL || strcmp(prev_line->line, current_line->line) != 0) {
      /* Print the line if unique */
      printf("%s", current_line->line);
    }
    /* Free the previous line */
    if (prev_line != NULL) {
      free(prev_line->line);
      free(prev_line);
    }
    prev_line = current_line;
  }
  /* Free the last line */
  if (current_line != NULL) {
    free(current_line->line);
    free(current_line);
  }
}

int main(int argc, char *argv[]) {
  uniq(stdin);
  return 0;
}