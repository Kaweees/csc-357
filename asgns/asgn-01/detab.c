#include <stdio.h>

#define TAB_WIDTH      8   // size of a tab
#define TAB_CHAR       9   // asci code for tab
#define BACKSPACE_CHAR 8   // asci code for a backspace
#define NEWLINE_CHAR   10  // asci code for a newline
#define CARRIAGE_CHAR  13  // asci code for a carriage return
#define SPACE_CHAR     32  // asci code for a space

int main(int argc, char *argv[]) {
  int ch;         // used to store the current char from stdin
  int count = 0;  // used to store amount of chars in current line
  while ((ch = getchar()) != EOF) {
    switch (ch) {
      case (TAB_CHAR): {
        for (int i = count % TAB_WIDTH; i < TAB_WIDTH; i++) {
          putchar(SPACE_CHAR);
          count++;  // increment count for each space
        }
        break;
      }
      case (NEWLINE_CHAR):
      case (CARRIAGE_CHAR): {
        putchar(ch);
        count = 0;  // reset count on new line
        break;
      }
      case (BACKSPACE_CHAR): {
        if (count) {
          putchar(ch);
          count--;  // decrement count for each backspace only if count > 0
        }
        break;
      }
      default: {
        putchar(ch);
        count++;
        break;
      }
    }
  }
  return 0;
}