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

void handleFileContents(int outfile, char* curr_path, int verbose, int strict) {
  struct stat* stat = safeMalloc(sizeof(struct stat));
  safeLstat(curr_path, stat);
  /* Process regular file */
  size_t file_size = stat->st_size;
  int infile = safeOpen(curr_path, O_RDONLY, 0);
  FileContent* file_contents = safeRead(infile);
  safeWrite(outfile, file_contents->file_contents, file_size);
  size_t padding_bytes = file_size % ARCHIVE_BLOCK_SIZE;
  if (padding_bytes > 0) {
    char* padding = (char*)safeCalloc(sizeof(char), padding_bytes);
    safeWrite(outfile, padding, padding_bytes);
    safeFree(padding);
  }
  freeFileContent(file_contents);
  safeClose(infile);
  safeFree(stat);
}

void handleDirContents(int outfile, char* curr_path, int verbose, int strict) {
  /* Process directory */
  DIR* dir = safeOpenDir(curr_path);
  DirContent* dir_contents = safeReadDir(dir);
  for (int i = 0; i < dir_contents->num_entries; i++) {
    struct dirent* entry = dir_contents->entries[i];
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    char* new_path = (char*)safeCalloc(
        sizeof(char), strlen(curr_path) + strlen(entry->d_name) + 2);
    snprintf(new_path, PATH_MAX, "%s/%s", curr_path, entry->d_name);
    createArchiveHelper(outfile, new_path, verbose, strict);
    safeFree(new_path);
  }
  // struct dirent* entry;
  // while ((entry = safeReadDir(dir)) != NULL) {
  //   if (strcmp(entry->d_name, ".") == 0 ||
  //       strcmp(entry->d_name, "..") == 0) {
  //     continue;
  //   }
  //   char* new_path = (char*)safeCalloc(sizeof(char), PATH_MAX);
  //   snprintf(new_path, PATH_MAX, "%s/%s", curr_path, entry->d_name);
  //   createArchiveHelper(outfile, new_path, verbose, strict);
  //   safeFree(new_path);
  // }
  safeCloseDir(dir);
  freeDirContent(dir_contents);
}

void handleLinkContents(int outfile, char* curr_path, int verbose, int strict) {
  // Process symbolic link
  char linkname[PATH_MAX];
  ssize_t r = readlink(curr_path, linkname, sizeof(linkname) - 1);
  if (!(r < 0)) {
    linkname[r] = '\0';
    struct stat* link_stat = safeMalloc(sizeof(struct stat));
    safeLstat(curr_path, link_stat);
    if (S_ISREG(link_stat->st_mode)) {
      handleFileContents(outfile, linkname, verbose, strict);
    } else if (S_ISDIR(link_stat->st_mode)) {
      handleDirContents(outfile, linkname, verbose, strict);
    } else if (S_ISLNK(link_stat->st_mode)) {
      handleLinkContents(outfile, linkname, verbose, strict);
    }
  }
}

void createArchiveHelper(
    int outfile, char* curr_path, int verbose, int strict) {
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
  /* Store the magic number in a string */
  char* header_magic = (char*)safeCalloc(sizeof(char), ARCHIVE_MAGIC_SIZE + 1);
  snprintf(header_magic, ARCHIVE_MAGIC_SIZE + 1, "%s", ARCHIVE_MAGIC);
  checksum += strlen(header_magic);
  /* Store the version in a string */
  char* header_version =
      (char*)safeCalloc(sizeof(char), ARCHIVE_VERSION_SIZE + 1);
  snprintf(header_version, ARCHIVE_VERSION_SIZE + 1, "%s", ARCHIVE_VERSION);
  checksum += strlen(header_version);
  /* Store the username in a string */
  char* header_uname = (char*)safeCalloc(sizeof(char), ARCHIVE_UNAME_SIZE);
  snprintf(header_uname, ARCHIVE_UNAME_SIZE, "%s", "cs537");
  checksum += strlen(header_uname);
  /* Store the group name in a string */
  char* header_gname = (char*)safeCalloc(sizeof(char), ARCHIVE_GNAME_SIZE);
  snprintf(header_gname, ARCHIVE_GNAME_SIZE, "%s", "cs537");
  checksum += strlen(header_gname);
  /* Store the devmajor in a string */
  char* header_devmajor =
      (char*)safeCalloc(sizeof(char), ARCHIVE_DEVMAJOR_SIZE + 1);
  snprintf(header_devmajor, ARCHIVE_DEVMAJOR_SIZE + 1, "%08o", 0);
  checksum += strlen(header_devmajor);
  /* Store the devminor in a string */
  char* header_devminor =
      (char*)safeCalloc(sizeof(char), ARCHIVE_DEVMINOR_SIZE + 1);
  snprintf(header_devminor, ARCHIVE_DEVMINOR_SIZE + 1, "%08o", 0);
  checksum += strlen(header_devminor);
  /* Store the prefix in a string */
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
  if (S_ISREG(stat->st_mode)) {
    handleFileContents(outfile, curr_path, verbose, strict);
  } else if (S_ISDIR(stat->st_mode)) {
    handleDirContents(outfile, curr_path, verbose, strict);
  } else if (S_ISLNK(stat->st_mode)) {
    handleLinkContents(outfile, curr_path, verbose, strict);
  }
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
    createArchiveHelper(outfile, file_names[0], verbose, strict);
  }
  safeClose(outfile);
}

/**
 * Lists the contents of a tar archive
 *
 * @param archive_name the name of the archive to list
 * @param verbose a flag to indicate whether to give verbose output while
 listing the archive
 * @param strict a flag to indicate whether to be strict on files conforming
 to the POSIX-specified USTAR archive format
 */
// void listArchive(char *archive_name, int verbose, int strict) {}

/**
 * Extracts the contents of a tar archive
 *
 * @param archive_name the name of the archive to extract
 * @param verbose a flag to indicate whether to give verbose output while
  extracting the archive
 * @param strict a flag to indicate whether to be strict on files conforming
 to
  the POSIX-specified USTAR archive format
*/
// void extractArchive(char *archive_name, int verbose, int strict) {}