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
  size_t line_length;
  /* The pointer to the line */
  char *line;
};

/**
 * Reads an arbitrarily long line from the given file into newly-allocated
 * space
 *
 * @param file - a pointer to the file to read the line from
 * @return a pointer to the line read from the file
 */
struct LongLine *read_long_line(FILE *file) {
  /* line read from buffer */
  struct LongLine *long_line =
      (struct LongLine *)safe_malloc(sizeof(struct LongLine));
  long_line->line_length = INITIAL_BUFFER_SIZE;
  /* Allocate valid space for the line read from buffer */
  long_line->line = (char *)safe_calloc(long_line->line_length, sizeof(char));
  int ch; /* character read from file */
  size_t buffer_size = 0;

  /* Read characters from the file until end of line or newline read */
  while ((ch = fgetc(file)) != EOF) {
    /* Reallocate space if buffer is full */
    if (buffer_size >= long_line->line_length -
                           1) { /* minus two because of the null character*/
      long_line->line_length *= 2;
      long_line->line =
          (char *)safe_realloc(long_line->line, long_line->line_length);
    }

    /* Add the character to the line */
    long_line->line[buffer_size++] =
        ch; /* increment line_length after adding character */
    if (ch != NEWLINE_CHAR && ch != CARRIAGE_CHAR) {
      /* Add a null terminator to the end of the line */
      long_line->line[buffer_size] = STRING_TERMINATOR;
      break;
    }
  }
  if (buffer_size) {
    long_line->line = (char *)safe_realloc(long_line->line, buffer_size);
    long_line->line_length = buffer_size;
    return long_line;
  } else {
    free(long_line->line);
    free(long_line);
    return NULL;
  }
}

void uniq(FILE *fp) {
  struct LongLine *old_line = NULL;
  struct LongLine *new_line;
  while ((new_line = read_long_line(fp)) != NULL) {
    if (old_line == NULL || strcmp(old_line->line, new_line->line) != 0) {
      /* Print the line if unique */

      printf("%s", new_line->line);
      /* Free the old line */
      if (old_line != NULL) {
        free(old_line->line);
        free(old_line);
      }

      /* Set the old line to the new line */
      old_line = new_line;
    } else {
      /* Free the new line */
      free(new_line->line);
      free(new_line);
    }
  }

  /* Free the old line */
  if (old_line != NULL) {
    free(old_line->line);
    free(old_line);
  }
}

int main(int argc, char *argv[]) {
  uniq(stdin);
  return 0;
}
