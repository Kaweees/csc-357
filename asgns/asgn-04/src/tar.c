#include "tar.h"

#include <fcntl.h>

#include "safe_alloc.h"
#include "safe_dir.h"
#include "safe_file.h"

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
void create_archive(char *archive_name, int file_count, char *file_names[],
    int verbose, int strict) {
  int outfile = safeOpen(archive_name, (O_WRONLY | O_CREAT | O_TRUNC), S_IRWXU);
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
  safeClose(outfile);
}

/**
 * Lists the contents of a tar archive
 *
 * @param archive_name the name of the archive to list
 * @param verbose a flag to indicate whether to give verbose output while
 listing the archive
 * @param strict a flag to indicate whether to be strict on files conforming to
 the POSIX-specified USTAR archive format
 */
void list_archive(char *archive_name, int verbose, int strict) {}

/**
 * Extracts the contents of a tar archive
 *
 * @param archive_name the name of the archive to extract
 * @param verbose a flag to indicate whether to give verbose output while
  extracting the archive
 * @param strict a flag to indicate whether to be strict on files conforming to
  the POSIX-specified USTAR archive format
*/
void extract_archive(char *archive_name, int verbose, int strict) {}