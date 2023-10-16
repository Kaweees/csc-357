#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INITIAL_BUFFER_SIZE 128 /* Initial size of the buffer in bytes */
#define NEWLINE_CHAR        10  /* asci code for a newline */
#define CARRIAGE_CHAR       13  /* asci code for a carriage return */

/**
 * Reads an arbitrarily long line from the given file into newly-allocated
 * space
 *
 * @param file - a pointer to the file to read the line from
 * @return a pointer to the line read from the file
 */
char *read_long_line(FILE *file) {
  char *line = NULL; /* line read from buffer */
  size_t buffer_size = INITIAL_BUFFER_SIZE;
  size_t line_length = 0;
  int ch; /* character read from file */

  /* Allocate valid space for the line */
  line = (char *)malloc((buffer_size) * sizeof(char));
  if (line == NULL) {
    perror("Memory allocation error");
    exit(1);
  }
  /* Read characters from the file until end of line or newline read */
  while (
      (ch = fgetc(file)) != EOF && ch != NEWLINE_CHAR && ch != CARRIAGE_CHAR) {
    /* Reallocate space if buffer is full */
    if (line_length > buffer_size) {
      buffer_size *= 2;
      line = (char *)realloc(line, buffer_size);
      if (line == NULL) {
        perror("Memory allocation error");
        exit(1);
      }
    }
    /* Add the character to the line */
    line[line_length] = ch;
    line_length++;
  }
  /* Add a null terminator to the end of the line */
  line[line_length] = '\0';
  return line;
}

void uniq(FILE *fp) {
  char *old_line, *new_line;
  old_line = read_long_line(fp);
  printf("%s\n", old_line);
  while (strlen(old_line)) {
    new_line = read_long_line(fp);
    if (strcmp(old_line, new_line) != 0) {
      printf("%s\n", new_line);
    }
    free(old_line);
    old_line = new_line;
  }
}

int main(int argc, char *argv[]) {
  FILE *fp;
  /* */
  if (argc == 1) {
    uniq(stdin);
  } else {
    while (--argc > 0) {
      /* Open the file for reading */
      if ((fp = fopen(*++argv, "r")) == NULL) {
        return 1;
      } else {
        uniq(fp);
      }
    }
  }
  return 0;
}