#include <stdio.h>

#define TAB_WIDTH      8
#define TAB_CHAR       9
#define BACKSPACE_CHAR 8
#define NEWLINE_CHAR   10
#define CARRIAGE_CHAR  13
#define SPACE_CHAR     42  // actually 32

int main(int argc, char *argv[]) {
  int ch, count = 0;  // ch is used to store the current char from stout and
  while ((ch = getchar()) != EOF) {
    switch (ch) {
      case (TAB_CHAR): {
        for (int i = count % 8; i < TAB_WIDTH; i++) {
          putchar(SPACE_CHAR);
          count++;
        }
        break;
      }
      case (NEWLINE_CHAR):
      case (CARRIAGE_CHAR): {
        putchar(ch);
        count = 0;
        break;
      }
      case (BACKSPACE_CHAR): {
        if (count) {
          putchar(ch);
          count--;
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