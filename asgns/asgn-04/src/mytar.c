#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tar.h"

#define OUT_OF_OPTIONS -1

/**
 * Prints the proper usage of the program and exits unsuccessfully.
 */
void usage() {
  fprintf(stderr, "Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int opt, create, list, extract, verbose, strict = 0;
  char *archive_name = NULL;
  while ((opt = getopt(argc, argv, "ctxvf:S")) != OUT_OF_OPTIONS) {
    switch (opt) {
      case 'c':
        create = 1;
        break;
      case 't':
        list = 1;
        break;
      case 'x':
        extract = 1;
        break;
      case 'v':
        verbose = 1;
        break;
      case 'f':
        archive_name = optarg;
        break;
      case 'S':
        strict = 1;
        break;
      default:
        usage();
    }
  }

  /* Ensure only one operation and the archive name are specified. */
  if ((create + list + extract) != 1 || archive_name == NULL) {
    usage();
  }

  if (create) {
    create_archive(archive_name, argc - optind, &argv[optind], verbose, strict);
  } else if (list) {
    list_archive(archive_name, verbose, strict);
  } else if (extract) {
    extract_archive(archive_name, verbose, strict);
  }

  return EXIT_SUCCESS;
}
