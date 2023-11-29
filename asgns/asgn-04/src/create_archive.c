#include "create_archive.h"

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
#define BUFFER_BLOCK_SIZE 4096
#define BLOCK_SIZE        512
#define PREFIX_SIZE       155
#define ID_MASK           07777777
#define MODE_MASK         07777
#define NAME_SIZE         100
#define OCTAL             8
/* Used if uid or gid is larger than the specified size */
int insert_octal(char *location, size_t size, int32_t val) {
  int ret = 0;
  if (val < 0 || (size < sizeof(val))) {
    ret++;
  } else {
    memset(location, 0, size);
    *(int32_t *)(location + size - sizeof(val)) = htonl(val);
    *location |= 0x80;
  }
  return ret;
}
/*Builds the fileHeader struct from the current path
 *   Each path has its own unique header*/
int buildHeaderFromPath(char *path, fileHeader *fh, int sCheck) {
  struct stat sb;
  struct passwd *uid = NULL;
  struct group *gid = NULL;
  unsigned char *header_ptr = (unsigned char *)fh;
  int i;
  unsigned int cSum = 0;
  if (lstat(path, &sb) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
  }
  if (strlen(path) <= NAME_SIZE) {
    strncpy(fh->name, path, NAME_SIZE);
  } else {
    printf("check\n");
    writeNamePrefix(fh, path);
    printf("prefix: %s\nname: %s\n", fh->prefix, fh->name);
  }
  /*Fills the fields of the header struct with the specified information
   *       that was extracted using the lstat function. */
  sprintf(fh->mode, "%07o", sb.st_mode & MODE_MASK);
  if (sb.st_uid > ID_MASK) {
    if (sCheck == 1) {
      perror("Unable to create conforming header.  Skipping.\n");
      return 0;
    }
    insert_octal(fh->uid, OCTAL, sb.st_uid);
  } else {
    sprintf(fh->uid, "%07o", sb.st_uid);
  }
  if (sb.st_gid > ID_MASK) {
    insert_octal(fh->gid, OCTAL, sb.st_gid);
  } else {
    sprintf(fh->gid, "%07o", sb.st_gid);
  }
  /*Sets the typeflags of the current path depending on
   *       whether it is a directory, link, or regular file*/
  if (S_ISREG(sb.st_mode)) {
    fh->typeflag = REGULAR_FILE;
    sprintf(fh->size, "%011lo", sb.st_size);
  } else if (S_ISDIR(sb.st_mode)) {
    fh->typeflag = DIRECTORY;
    strcpy(fh->size, "00000000000");
  } else if (S_ISLNK(sb.st_mode)) {
    fh->typeflag = SYM_LINK;
    if ((readlink(path, fh->linkname, sizeof(fh->linkname))) == -1) {
      perror("symbolic link");
    }
    strcpy(fh->size, "00000000000");
  } else {
    perror("unknown file type");
  }
  sprintf(fh->mtime, "%011lo", sb.st_mtime);
  strcpy(fh->magic, "ustar\0");
  memcpy(fh->version, "00", 2);
  uid = getpwuid(sb.st_uid);
  gid = getgrgid(sb.st_gid);
  strcpy(fh->uname, uid->pw_name);
  strcpy(fh->gname, gid->gr_name);
  for (i = 0; i < BLOCK_SIZE; i++) {
    cSum += (unsigned char)*header_ptr;
    header_ptr++;
  }
  cSum += ' ' * OCTAL;
  sprintf(fh->chksum, "%07o", cSum);
  return 1;
}
/*Archives the file, by writing the contents of it
 *   to the body. */
void archiveFile(char *path, FILE *outputFile) {
  FILE *inputFile;
  char buffer[BUFFER_BLOCK_SIZE] = {0};
  if ((inputFile = fopen(path, "rb")) == NULL) {
    perror("file error");
    return;
  }
  while (fread(buffer, BLOCK_SIZE, 1, inputFile)) {
    fwrite(buffer, BLOCK_SIZE, 1, outputFile);
    memset(buffer, 0, BUFFER_BLOCK_SIZE);
  }
  fwrite(buffer, BLOCK_SIZE, 1, outputFile);
  fclose(inputFile);
  return;
}
/*Writes the prefix of the path if neeeded. Only used
 *  if the name is larger than 100, seperates the path from the main*/
void writeNamePrefix(fileHeader *fh, char *path) {
  int i;
  for (i = strlen(path) - 1; i >= 0; i--) {
    if (path[i] == '/') {
      break;
    }
  }
  strncpy(fh->prefix, path, i);
  if (i <= PREFIX_SIZE - 1) {
    fh->prefix[i] = '\0';
  }
  strncpy(fh->name, path + i + 1, strlen(path) - i);
}
