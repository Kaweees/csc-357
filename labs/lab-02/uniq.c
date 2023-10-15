#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INITIAL_BUFFER_SIZE 128 /* Initial buffer size in bytes */
#define NEWLINE_CHAR        10  /* asci code for a newline */
#define CARRIAGE_CHAR       13  /* asci code for a carriage return */

char *read_long_line(FILE *file) {
  /* Reads an arbitrarily long line from the given file into newly-allocated
   * space */
  int ch; /* character read from file */
  char *line = (char *)malloc(
      (INITIAL_BUFFER_SIZE) * sizeof(char)); /* line read from buffer */
  int buffer_size = 0;                       /* current size of the buffer */
  if (file == NULL) {
    return NULL;
  }
  while ((ch = fgetc(file)) != EOF) {
    buffer_size++;
  }
  return line;
}

void uniq(FILE *fp) {
  char *old_line, *new_line;

  old_line = read_long_line(fp);

  /* Read and process characters from the file one at a time */
  // Read
  while ((character = fgetc(file)) != EOF) {
    // Process the character (you can replace this with your own logic)
    printf("%c", (char)character);
  }

  while (fgetsc(line, sizeof(line), file) != NULL) {
    printf(
        "Line: %s", line);  // You can replace this with your processing logic
  }

  fre

      return;
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

    return 0;
  }
}