#include "list_archive.h"

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
#include "extract_archive.h"
#define BUFFER_BLOCK_SIZE 4096
#define BLOCK_SIZE        512
#define PATH_SIZE         256
#define NAME_SIZE         100
#define PREFIX_SIZE       155
#define OCTAL             8
/*Lists the archive, and controls the flow of the table of contents
 *   flag specified within the command line arguments. */
void listArchive(int argc, char *argv[], int vCheck, int sCheck) {
  FILE *archive;
  fileHeader *fH;
  int blocksToSkip;
  int readChars = 0;
  unsigned int size;
  char *name;
  int i;
  /* opens archive file */
  if ((archive = fopen(argv[2], "rb")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  /* allocates header struct */
  if (!(fH = (fileHeader *)calloc(sizeof(fileHeader), 1))) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }
  /* loops through archive file */
  while ((readChars = fread(fH, 1, BLOCK_SIZE, archive)) > 0) {
    if (ferror(archive)) {
      perror("reading archive\n");
      exit(EXIT_FAILURE);
    }
    if (checkNullBlock(fH) == 1) {
      free(fH);
      return;
    }
    checkChksum(fH);
    if (sCheck == 1) {
      if (fH->uid[0] & (1 << 7)) {
        free(fH);
        printf("Malformed header found.  Bailing.\n");
        return;
      }
    }
    name = getName(fH);
    if (argc == 3) {
      if (vCheck == 0) {
        printName(fH);
      } else if (vCheck == 1) {
        writeHeader(fH);
      }
    } else {
      for (i = 3; i < argc; i++) {
        if (strncmp(argv[i], name, strlen(argv[i])) == 0) {
          if (vCheck == 0) {
            printName(fH);
          } else if (vCheck == 1) {
            writeHeader(fH);
          }
        }
      }
    }
    free(name);
    size = strtoul(fH->size, NULL, OCTAL);
    blocksToSkip = (((size / BLOCK_SIZE) + ((size % BLOCK_SIZE) == 0 ? 0 : 1)) *
                    BLOCK_SIZE);
    fseek(archive, blocksToSkip, SEEK_CUR);
  }
  free(fH);
  fclose(archive);
}
/*This function checks the NULL block, if it is or is not NULL*/
int checkNullBlock(fileHeader *fH) {
  unsigned int sum = 0;
  unsigned char *headerPtr = (unsigned char *)fH;
  int i;
  for (i = 0; i < BLOCK_SIZE; i++) {
    sum += (unsigned char)*headerPtr;
    headerPtr++;
  }
  return (sum == 0);
}
/*This function is used to check the chksum
 *   checking for correctness. */
void checkChksum(fileHeader *fH) {
  int chkSum = strtoul(fH->chksum, NULL, OCTAL);
  int i;
  unsigned int sum = 0;
  unsigned char *headerPtr = (unsigned char *)fH;
  for (i = 0; i < BLOCK_SIZE; i++) {
    if (i < PREFIX_SIZE - 7 || i > PREFIX_SIZE) {
      sum += (unsigned char)*headerPtr;
    }
    headerPtr++;
  }
  sum += ' ' * OCTAL;
  if (chkSum != sum) {
    perror("Bad tarfile");
    exit(EXIT_FAILURE);
  }
}
/*This function writes the header given the fileHeader.
 *   Used when the verbose flag is specified */
void writeHeader(fileHeader *fH) {
  char *permissions = (char *)malloc(11);
  char mtimeString[17];
  time_t mtime = strtoul(fH->mtime, NULL, OCTAL);
  struct tm *time = NULL;
  char *name;
  if (!(name = (char *)malloc(strlen(fH->uname) + strlen(fH->gname) + 2))) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  /* adds name and groupname to name */
  strcpy(name, fH->uname);
  strcat(name, "/");
  strcat(name, fH->gname);
  /* gets modified time */
  time = localtime(&mtime);
  sprintf(mtimeString, "%04i-%02i-%02i %02i:%02i", 1900 + time->tm_year,
      time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min);
  makePermissions(fH, permissions);
  printf("%s ", permissions);
  printf("%17s ", name);
  printf("%8i ", (int)strtoul(fH->size, NULL, OCTAL));
  printf("%16s ", mtimeString);
  printName(fH);
  free(permissions);
  free(name);
}
/*Makes the permissions from the fileHeader, used for printing
 *   when the verbose flag is specified*/
void makePermissions(fileHeader *fH, char *permissions) {
  int mode;
  mode = strtoul(fH->mode, NULL, OCTAL);
  if (fH->typeflag == DIRECTORY) {
    permissions[0] = 'd';
  } else if (fH->typeflag == SYM_LINK) {
    permissions[0] = 'l';
  } else {
    permissions[0] = '-';
  }
  permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
  permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
  permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
  permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
  permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
  permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
  permissions[7] = (mode & S_IROTH) ? 'r' : '-';
  permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
  permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
  permissions[10] = '\0';
}
/*Used to print the names, extra functionality is added
 *   to make sure that the prefix and the name is correctly
 *     concatenated and added. */
void printName(fileHeader *fH) {
  char *fullName;
  if (fH->prefix[0] == '\0' && fH->name[NAME_SIZE - 1] == '\0') {
    printf("%s\n", fH->name);
    return;
  } else {
    if (fH->prefix[0] == '\0') {
      if (!(fullName = (char *)malloc(NAME_SIZE + 1))) {
        perror("malloc");
        exit(EXIT_FAILURE);
      }
      strncpy(fullName, fH->name, NAME_SIZE);
      fullName[NAME_SIZE] = '\0';
    } else {
      if (!(fullName = (char *)malloc(PATH_SIZE))) {
        perror("malloc");
        exit(EXIT_FAILURE);
      }
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
    printf("%s\n", fullName);
    free(fullName);
  }
}
