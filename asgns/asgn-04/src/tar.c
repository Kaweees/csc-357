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