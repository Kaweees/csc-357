#include "extract_archive.h"

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <math.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "create_archive.h"
#include "list_archive.h"
#define BUFFER_BLOCK_SIZE 4096
#define BLOCK_SIZE        512
#define PATH_SIZE         256
#define NAME_SIZE         100
#define PREFIX_SIZE       155
#define OCTAL             8
/* given the command line arguments, loops through tarfile
 * and either extracts all files or finds and extracts specific
 * given files
 * */
void extractArchive(int argc, char *argv[], int vCheck, int sCheck) {
  FILE *archive;
  fileHeader *fH;
  int readChars = 0, i, foundCheck, size, blocksToSkip;
  char *name;
  /*opens tar file */
  archive = fopen(argv[2], "rb");
  /*allocates space for header struct */
  if (!(fH = (fileHeader *)calloc(sizeof(fileHeader), 1))) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }
  /*loops through blocks of tar file */
  while ((readChars = fread(fH, 1, BLOCK_SIZE, archive)) > 0) {
    if (strtoul(fH->chksum, NULL, OCTAL) == 0) {
      break;
    }
    /*checks if hit null block */
    if (checkNullBlock(fH) == 1) {
      free(fH);
      return;
    }
    /*checks if tarfile corrupted through chksum */
    checkChksum(fH);
    /*error reading archive */
    if (ferror(archive)) {
      perror("reading archive\n");
      exit(EXIT_FAILURE);
    }
    /*strict check, checks user id */
    if (sCheck == 1) {
      if (fH->uid[0] & (1 << 7)) {
        free(fH);
        printf("Malformed header found.  Bailing.\n");
        return;
      }
    }
    /* gets whole name of file from header */
    name = getName(fH);
    if (argc <= 3) /*case where no names given, extracts whole tarfile */
    {
      extractFile(fH, name, archive, vCheck);
    } else if (argc >= 4) /*names given, searches for names */
    {
      foundCheck = 0;
      for (i = 3; i < argc; i++) {
        /* checks if path is part of searched file */
        if ((name[strlen(name) - 1] == '/' &&
                strncmp(name, argv[i], strlen(name)) == 0) ||
            (name[strlen(name) - 1] != '/' && strcmp(name, argv[i]) == 0) ||
            (strncmp(argv[i], name, strlen(argv[i])) == 0)) {
          foundCheck = 1;
          extractFile(fH, name, archive, vCheck);
        }
      }
      /* if file not extracted, seeks rest of data to get to header*/
      if (foundCheck == 0) {
        size = strtoul(fH->size, NULL, OCTAL);
        blocksToSkip =
            (((size / BLOCK_SIZE) + ((size % BLOCK_SIZE) == 0 ? 0 : 1)) *
                BLOCK_SIZE);
        fseek(archive, blocksToSkip, SEEK_CUR);
      }
    }
    free(name);
  }
  free(fH);
  fclose(archive);
}
/* given file header, processes name and prefix and
 * checks each case to return a pointer to a new string
 * of the combined full name
 * */
char *getName(fileHeader *fH) {
  char *fullName;
  if (fH->prefix[0] == '\0' && fH->name[NAME_SIZE - 1] == '\0') {
    if (!(fullName = (char *)malloc(strlen(fH->name) + 1))) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    strcpy(fullName, fH->name);
  } else {
    if (fH->prefix[0] == '\0') {
      if (!(fullName = (char *)malloc(NAME_SIZE + 1))) {
        perror("malloc");
        exit(EXIT_FAILURE);
      }
      strncpy(fullName, fH->name, NAME_SIZE);
      fullName[NAME_SIZE] = '\0';
    } else {
      fullName = (char *)malloc(PATH_SIZE);
      if (fH->prefix[PREFIX_SIZE - 1] != '\0') {
        strncpy(fullName, fH->prefix, PREFIX_SIZE);
        fullName[PREFIX_SIZE] = '\0';
      } else {
        strcpy(fullName, fH->prefix);
      }
      strcat(fullName, "/");
      if (fH->name[NAME_SIZE - 1] == '\0') {
        strcat(fullName, fH->name);
      } else {
        fullName[strlen(fullName) + NAME_SIZE] = '\0';
        strncat(fullName, fH->name, NAME_SIZE);
      }
    }
  }
  return fullName;
}
/*given file name, has certain process for type of file
 * then extracts
 * */
void extractFile(fileHeader *fH, char *name, FILE *archive_file, int vCheck) {
  if (vCheck == 1) {
    printf("%s\n", name);
  }
  if (fH->typeflag == REGULAR_FILE) {
    regularExtract(fH, name, archive_file);
  } else if (fH->typeflag == DIRECTORY) {
    directoryExtract(fH, name);
  } else if (fH->typeflag == SYM_LINK) {
    linkExtract(fH, name);
  }
}
/* if regular file, opens file and writes data to opened file */
void regularExtract(fileHeader *fH, char *name, FILE *archive_file) {
  FILE *oFile;
  char readBlock[BLOCK_SIZE];
  int totalChars = 0, readChars = 0;
  int size;
  size = strtoul(fH->size, NULL, OCTAL);
  if ((oFile = fopen(name, "w")) == NULL) {
    fprintf(stderr, "can't open %s: %s\n", name, strerror(errno));
    return;
  }
  while (totalChars < size) {
    readChars = fread(readBlock, 1, BLOCK_SIZE, archive_file);
    if ((size - totalChars) <= BLOCK_SIZE) {
      readChars = (size - totalChars);
    } else {
      readChars = BLOCK_SIZE;
    }
    totalChars += readChars;
    fwrite(readBlock, readChars, 1, oFile);
    memset(readBlock, 0, BLOCK_SIZE);
  }
  chmod(name, strtoul(fH->mode, NULL, OCTAL));
  chown(name, strtoul(fH->uid, NULL, OCTAL), strtoul(fH->gid, NULL, OCTAL));
  fclose(oFile);
}
/* if extracting directory, creates directory from name */
void directoryExtract(fileHeader *fH, char *name) {
  mkdir(name, strtoul(fH->mode, NULL, OCTAL));
  chmod(name, strtoul(fH->mode, NULL, OCTAL));
  chown(name, strtoul(fH->uid, NULL, OCTAL), strtoul(fH->gid, NULL, OCTAL));
}
/* if extracting link, creates link from linkname to file name */
void linkExtract(fileHeader *fH, char *name) {
  symlink(fH->linkname, name);
  chmod(name, strtoul(fH->mode, NULL, 8));
  chown(name, strtoul(fH->uid, NULL, 8), strtoul(fH->gid, NULL, 8));
}
