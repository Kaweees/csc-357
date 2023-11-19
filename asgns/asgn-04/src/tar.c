#include "tar.h"

#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

#include "safe_alloc.h"
#include "safe_dir.h"
#include "safe_file.h"


// char curr_dir[PATH_MAX];
// safeGetCwd(curr_dir, PATH_MAX);

void createArchiveHelper(int outfile, char* curr_path) {
  struct stat* stat = safeMalloc(sizeof(struct stat));
  safeLstat(curr_path, stat);
  char * header_name = (char*) safeCalloc(sizeof(char), ARCHIVE_NAME_SIZE);
  strncpy(header_name, curr_path, ARCHIVE_NAME_SIZE);
  /* Write the string to the archive */
  safeWrite(outfile, header_name, ARCHIVE_NAME_SIZE);
  safeFree(header_name);
  char * header_mode = (char*) safeCalloc(sizeof(char), ARCHIVE_MODE_SIZE);
  snprintf(header_mode, ARCHIVE_MODE_SIZE, "%08o", stat->st_mode);
  /* Write the mode to the archive */
  safeWrite(outfile, header_mode, ARCHIVE_MODE_SIZE);
  safeFree(header_mode);
  char * header_uid = (char*) safeCalloc(sizeof(char), ARCHIVE_UID_SIZE);
  snprintf(header_uid, ARCHIVE_UID_SIZE, "%08o", stat->st_uid);
  /* Write the uid to the archive */
  safeWrite(outfile, header_uid, ARCHIVE_UID_SIZE);
  safeFree(header_uid);
  char * header_gid = (char*) safeCalloc(sizeof(char), ARCHIVE_GID_SIZE);
  snprintf(header_gid, ARCHIVE_GID_SIZE, "%08o", stat->st_gid);
  /* Write the gid to the archive */
  safeWrite(outfile, header_gid, ARCHIVE_GID_SIZE);
  safeFree(header_gid);
  char * header_size = (char*) safeCalloc(sizeof(char), ARCHIVE_SIZE_SIZE);
  snprintf(header_size, ARCHIVE_SIZE_SIZE, "%012o", stat->st_size);
  /* Write the size to the archive */
  safeWrite(outfile, header_size, ARCHIVE_SIZE_SIZE);
  safeFree(header_size);
  char * header_mtime = (char*) safeCalloc(sizeof(char), ARCHIVE_MTIME_SIZE);
  snprintf(header_mtime, ARCHIVE_MTIME_SIZE, "%012o", stat->st_mtime);
  /* Write the mtime to the archive */
  safeWrite(outfile, header_mtime, ARCHIVE_MTIME_SIZE);
  safeFree(header_mtime);
  char * header_chksum = (char*) safeCalloc(sizeof(char), ARCHIVE_CHKSUM_SIZE);
  snprintf(header_chksum, ARCHIVE_CHKSUM_SIZE, "%08o", 0);
  
  

  // for (int i = 0; i < cwd_contents->num_entries; i++) {
  //   struct dirent* cwd_entry = cwd_contents->entries[i];
  //   struct stat* stat = safeMalloc(sizeof(struct stat));
  //   safeLstat(cwd_entry->d_name, stat);
  //   // Get the length of the string
  //   size_t inputLength = strlen(cwd_entry->d_name);
  //   // Determine the number of bytes to write (maximum 100)
  //   size_t bytesToWrite = (inputLength > ARCHIVE_NAME_SIZE) ? ARCHIVE_NAME_SIZE : inputLength;
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
    // /* Write the contents of the archive block */
    // safeWrite(outfile, &header, ARCHIVE_BLOCK_SIZE);
    
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
void createArchive(char *archive_name, int file_count, char *file_names[],
    int verbose, int strict) {
  int outfile = safeOpen(archive_name, (O_WRONLY | O_CREAT | O_TRUNC), S_IRWXU);
  for (int i = 0; i < file_count; i++) {
    printf("file_names: %s\n", file_names[i]);
    createArchiveHelper(outfile, file_names[0]);
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
//  * @param strict a flag to indicate whether to be strict on files conforming to
//  the POSIX-specified USTAR archive format
//  */
// void listArchive(char *archive_name, int verbose, int strict) {}

// /**
//  * Extracts the contents of a tar archive
//  *
//  * @param archive_name the name of the archive to extract
//  * @param verbose a flag to indicate whether to give verbose output while
//   extracting the archive
//  * @param strict a flag to indicate whether to be strict on files conforming to
//   the POSIX-specified USTAR archive format
// */
// void extractArchive(char *archive_name, int verbose, int strict) {}