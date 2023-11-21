#include "tar.h"

#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "safe_alloc.h"
#include "safe_dir.h"
#include "safe_file.h"

void createArchiveHelper(
    int outfile, char* curr_path, int verbose, int strict) {
  printf("strict: %d\n", strict);
  uint8_t checksum = 0;
  /* Get the stat of the file/directory */
  struct stat* stat = safeMalloc(sizeof(struct stat));
  safeLstat(curr_path, stat);
  /* Store the name in a string */
  char* header_name = (char*)safeCalloc(sizeof(char), ARCHIVE_NAME_SIZE);
  strncpy(header_name, curr_path, ARCHIVE_NAME_SIZE);
  checksum += strlen(header_name);
  /* Store the mode in a string */
  char* header_mode = (char*)safeCalloc(sizeof(char), ARCHIVE_MODE_SIZE + 1);
  snprintf(header_mode, ARCHIVE_MODE_SIZE + 1, "%08o", stat->st_mode);
  checksum += strlen(header_mode);
  /* Store the user id in a string */
  char* header_uid = (char*)safeCalloc(sizeof(char), ARCHIVE_UID_SIZE + 1);
  snprintf(header_uid, ARCHIVE_UID_SIZE + 1, "%08o", stat->st_uid);
  checksum += strlen(header_uid);
  /* Store the group id in a string */
  char* header_gid = (char*)safeCalloc(sizeof(char), ARCHIVE_GID_SIZE + 1);
  snprintf(header_gid, ARCHIVE_GID_SIZE + 1, "%08o", stat->st_gid);
  checksum += strlen(header_gid);
  /* Store the size in a string */
  char* header_size = (char*)safeCalloc(sizeof(char), ARCHIVE_SIZE_SIZE + 1);
  snprintf(
      header_size, ARCHIVE_SIZE_SIZE + 1, "%012o", (unsigned int)stat->st_size);
  checksum += strlen(header_size);
  /* Store the mtime in a string */
  char* header_mtime = (char*)safeCalloc(sizeof(char), ARCHIVE_MTIME_SIZE + 1);
  snprintf(header_mtime, ARCHIVE_MTIME_SIZE + 1, "%012o",
      (unsigned int)stat->st_mtime);
  checksum += strlen(header_mtime);
  /* Store the typeflag in a string */
  FileType header_typeflag;
  if (S_ISREG(stat->st_mode)) {
    header_typeflag = REGULAR_FILE;
  } else if (S_ISLNK(stat->st_mode)) {
    header_typeflag = SYMBOLIC_LINK;
  } else if (S_ISDIR(stat->st_mode)) {
    header_typeflag = DIRECTORY;
  } else {
    header_typeflag = REGULAR_FILE_ALTERNATE;
  }
  checksum++;
  /* Store the linkname in a string */
  char* header_linkname =
      (char*)safeCalloc(sizeof(char), ARCHIVE_LINKNAME_SIZE);
  snprintf(header_linkname, ARCHIVE_LINKNAME_SIZE, "%s", "");
  checksum += strlen(header_linkname);
  /* Store the magic in a string */
  char* header_magic = (char*)safeCalloc(sizeof(char), ARCHIVE_MAGIC_SIZE + 1);
  snprintf(header_magic, ARCHIVE_MAGIC_SIZE + 1, "%s", ARCHIVE_MAGIC);
  checksum += strlen(header_magic);
  char* header_version =
      (char*)safeCalloc(sizeof(char), ARCHIVE_VERSION_SIZE + 1);
  snprintf(header_version, ARCHIVE_VERSION_SIZE + 1, "%s", ARCHIVE_VERSION);
  checksum += strlen(header_version);
  char* header_uname = (char*)safeCalloc(sizeof(char), ARCHIVE_UNAME_SIZE);
  snprintf(header_uname, ARCHIVE_UNAME_SIZE, "%s", "cs537");
  checksum += strlen(header_uname);
  char* header_gname = (char*)safeCalloc(sizeof(char), ARCHIVE_GNAME_SIZE);
  snprintf(header_gname, ARCHIVE_GNAME_SIZE, "%s", "cs537");
  checksum += strlen(header_gname);
  char* header_devmajor =
      (char*)safeCalloc(sizeof(char), ARCHIVE_DEVMAJOR_SIZE + 1);
  snprintf(header_devmajor, ARCHIVE_DEVMAJOR_SIZE + 1, "%08o", 0);
  checksum += strlen(header_devmajor);
  char* header_devminor =
      (char*)safeCalloc(sizeof(char), ARCHIVE_DEVMINOR_SIZE + 1);
  snprintf(header_devminor, ARCHIVE_DEVMINOR_SIZE + 1, "%08o", 0);
  checksum += strlen(header_devminor);
  char* header_prefix = (char*)safeCalloc(sizeof(char), ARCHIVE_PREFIX_SIZE);
  snprintf(header_prefix, ARCHIVE_PREFIX_SIZE, "%s", "");
  checksum += strlen(header_prefix);
  /* Store the chksum in a string */
  char* header_chksum =
      (char*)safeCalloc(sizeof(char), ARCHIVE_CHKSUM_SIZE + 1);
  checksum += ARCHIVE_CHKSUM_SIZE;
  snprintf(header_chksum, ARCHIVE_CHKSUM_SIZE + 1, "%08o", 0);
  /* Write the string to the archive */
  safeWrite(outfile, header_name, ARCHIVE_NAME_SIZE);
  safeFree(header_name);
  /* Write the mode to the archive */
  safeWrite(outfile, header_mode, ARCHIVE_MODE_SIZE);
  safeFree(header_mode);
  /* Write the uid to the archive */
  safeWrite(outfile, header_uid, ARCHIVE_UID_SIZE);
  safeFree(header_uid);
  /* Write the gid to the archive */
  safeWrite(outfile, header_gid, ARCHIVE_GID_SIZE);
  safeFree(header_gid);
  /* Write the size to the archive */
  safeWrite(outfile, header_size, ARCHIVE_SIZE_SIZE);
  safeFree(header_size);
  /* Write the mtime to the archive */
  safeWrite(outfile, header_mtime, ARCHIVE_MTIME_SIZE);
  safeFree(header_mtime);
  /* Write the chksum to the archive */
  safeWrite(outfile, header_chksum, ARCHIVE_CHKSUM_SIZE);
  safeFree(header_chksum);
  /* Write the typeflag to the archive */
  safeWrite(outfile, &header_typeflag, ARCHIVE_TYPEFLAG_SIZE);
  /* Write the linkname to the archive */
  safeWrite(outfile, header_linkname, ARCHIVE_LINKNAME_SIZE);
  safeFree(header_linkname);
  /* Write the magic to the archive */
  safeWrite(outfile, header_magic, ARCHIVE_MAGIC_SIZE);
  safeFree(header_magic);
  /* Write the version to the archive */
  safeWrite(outfile, header_version, ARCHIVE_VERSION_SIZE);
  safeFree(header_version);
  /* Write the uname to the archive */
  safeWrite(outfile, header_uname, ARCHIVE_UNAME_SIZE);
  safeFree(header_uname);
  /* Write the gname to the archive */
  safeWrite(outfile, header_gname, ARCHIVE_GNAME_SIZE);
  safeFree(header_gname);
  /* Write the devmajor to the archive */
  safeWrite(outfile, header_devmajor, ARCHIVE_DEVMAJOR_SIZE);
  safeFree(header_devmajor);
  /* Write the devminor to the archive */
  safeWrite(outfile, header_devminor, ARCHIVE_DEVMINOR_SIZE);
  safeFree(header_devminor);
  /* Write the prefix to the archive */
  safeWrite(outfile, header_prefix, ARCHIVE_PREFIX_SIZE);
  safeFree(header_prefix);
  /* print out file permissions, the owner/group, the size, last modification
   * time and the filename*/
  if (verbose) {
    struct passwd* pwd = getpwuid(stat->st_uid);
    struct group* grp = getgrgid(stat->st_gid);
    char time_str[MTIME_WIDTH + 1] = {0};
    char owner_group[OWNER_GROUP_WIDTH + 1];
    snprintf(owner_group, OWNER_GROUP_WIDTH + 1, "%s/%s", pwd->pw_name,
        grp->gr_name);
    strftime(time_str, MTIME_WIDTH + 1, "%Y-%m-%d %H:%M",
        localtime(&stat->st_mtime));
    printf("%c%c%c%c%c%c%c%c%c%c ",
        (S_ISDIR(stat->st_mode))   ? 'd'
        : (S_ISLNK(stat->st_mode)) ? 'l'
                                   : '-',
        (stat->st_mode & S_IRUSR) ? 'r' : '-',
        (stat->st_mode & S_IWUSR) ? 'w' : '-',
        (stat->st_mode & S_IXUSR) ? 'x' : '-',
        (stat->st_mode & S_IRGRP) ? 'r' : '-',
        (stat->st_mode & S_IWGRP) ? 'w' : '-',
        (stat->st_mode & S_IXGRP) ? 'x' : '-',
        (stat->st_mode & S_IROTH) ? 'r' : '-',
        (stat->st_mode & S_IWOTH) ? 'w' : '-',
        (stat->st_mode & S_IXOTH) ? 'x' : '-');
    printf("%-*s", OWNER_GROUP_WIDTH, owner_group);
    printf(" %*lu", SIZE_WIDTH, (unsigned long)stat->st_size);
    printf(" %-*s", MTIME_WIDTH, time_str);
    printf(" %s\n", curr_path);
  }

  // for (int i = 0; i < cwd_contents->num_entries; i++) {
  //   struct dirent* cwd_entry = cwd_contents->entries[i];
  //   struct stat* stat = safeMalloc(sizeof(struct stat));
  //   safeLstat(cwd_entry->d_name, stat);
  //   // Get the length of the string
  //   size_t inputLength = strlen(cwd_entry->d_name);
  //   // Determine the number of bytes to write (maximum 100)
  //   size_t bytesToWrite = (inputLength > ARCHIVE_NAME_SIZE) ?
  //   ARCHIVE_NAME_SIZE : inputLength;
  //   // Write the string to the archive
  //   safeWrite(outfile, cwd_entry->d_name, bytesToWrite);
  //   // Write the padding to the archive
  //   char padding[ARCHIVE_NAME_SIZE - bytesToWrite];
  //   safeWrite(outfile, padding, ARCHIVE_NAME_SIZE - bytesToWrite);
  // }

  // USTARHeader* header = (USTARHeader*) safeCalloc(sizeof(USTARHeader), 1);
  // header->name = cwd_entry->d_name;
  // header->mode = stat->st_mode;
  // header->uid = stat->st_uid;
  // header->gid = stat->st_gid;
  // if (S_ISREG(file_stat.st_mode)) {
  //   // Process regular file
  // } else if (S_ISDIR(file_stat.st_mode)) {
  //   // Process directory
  // } else if (S_ISLNK(file_stat.st_mode)) {
  //   // Process symbolic link
  // }

  // header->size = stat->st_size;
  // header->mtime = stat->st_mtime;
  // header->chksum =
  // header->typeflag = 0;
  // header->linkname = "";
  // header->magic = TMAGIC;
  // header->version = TVERSION;
  // header->uname = "cs537";
  // header->gname = "cs537";
  // header->devmajor = 0;
  // header->devminor = 0;
  // header->prefix = "";
  // header->pad = "";

  // }
}

/**
 * Creates a tar archive
 *
 * @param archive_name the name of the archive to create
 * @param file_count the number of files to archive
 * @param file_names an array of file names to archive
 * @param verbose a flag to indicate whether to give verbose output while
 creating the archive
 * @param strict a flag to indicate whether to be strict on files conforming
 to the POSIX-specified USTAR archive format
 */
void createArchive(char* archive_name, int file_count, char* file_names[],
    int verbose, int strict) {
  int outfile = safeOpen(archive_name, (O_WRONLY | O_CREAT | O_TRUNC), S_IRWXU);
  for (int i = 0; i < file_count; i++) {
    printf("file_names: %s\n", file_names[i]);
    createArchiveHelper(outfile, file_names[0], verbose, strict);
  }
  safeClose(outfile);

  // for (int i = 0; i < file_count; i++) {
  //   int infile = safeOpen(file_names[i], O_RDONLY, 0);
  //   struct stat stat_buf;
  //   safeFstat(infile, &stat_buf);
  //   struct posix_header header;
  //   memset(&header, 0, sizeof(header));
  //   strncpy(header.name, file_names[i], sizeof(header.name));
  //   snprintf(header.size, sizeof(header.size), "%011llo", stat_buf.st_size);
  //   snprintf(header.mode, sizeof(header.mode), "%07o", stat_buf.st_mode);
  //   snprintf(header.mtime, sizeof(header.mtime), "%011lo",
  //   stat_buf.st_mtime); snprintf(header.chksum, sizeof(header.chksum),
  //   "%06o", 0); strncpy(header.magic, TMAGIC, sizeof(header.magic));
  //   strncpy(header.version, TVERSION, sizeof(header.version));
  //   strncpy(header.uname, "cs537", sizeof(header.uname));
  //   strncpy(header.gname, "cs537", sizeof(header.gname));
  //   snprintf(header.devmajor, sizeof(header.devmajor), "%07o", 0);
  //   snprintf(header.devminor, sizeof(header.devminor), "%07o", 0);
  //   safeWrite(outfile, &header, sizeof(header));
  //   char buf[512];
  //   int bytes_read;
  //   while ((bytes_read = safeRead(infile, buf, sizeof(buf))) > 0) {
  //     safeWrite(outfile, buf, bytes_read);
  //   }
  //   if (bytes_read < 0) {
  //     fprintf(stderr, "Error reading file: %s\n", file_names[i]);
  //     exit(EXIT_FAILURE);
  //   }
  //   if (bytes_read < sizeof(buf)) {
  //     memset(buf, 0, sizeof(buf));
  //     safeWrite(outfile, buf, sizeof(buf) - bytes_read);
  //   }
}

// /**
//  * Lists the contents of a tar archive
//  *
//  * @param archive_name the name of the archive to list
//  * @param verbose a flag to indicate whether to give verbose output while
//  listing the archive
//  * @param strict a flag to indicate whether to be strict on files conforming
//  to the POSIX-specified USTAR archive format
//  */
// void listArchive(char *archive_name, int verbose, int strict) {}

// /**
//  * Extracts the contents of a tar archive
//  *
//  * @param archive_name the name of the archive to extract
//  * @param verbose a flag to indicate whether to give verbose output while
//   extracting the archive
//  * @param strict a flag to indicate whether to be strict on files conforming
//  to
//   the POSIX-specified USTAR archive format
// */
// void extractArchive(char *archive_name, int verbose, int strict) {}