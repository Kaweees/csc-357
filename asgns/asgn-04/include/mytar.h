#ifndef MY_TAR_H
#define MY_TAR_H

#define ARCHIVE_BLOCK_SIZE    512 /* Block within the archive */
#define ARCHIVE_HEADER_SIZE   512 /* The size of an archive header */
#define ARCHIVE_NAME_SIZE     100 /* File name portion of the header */
#define ARCHIVE_MODE_SIZE     8   /* File modes portion of the header */
#define ARCHIVE_UID_SIZE      8   /* User id portion of the header */
#define ARCHIVE_GID_SIZE      8   /* Group id portion of the header */
#define ARCHIVE_SIZE_SIZE     12  /* File size portion of the header */
#define ARCHIVE_MTIME_SIZE    12  /* Modification time portion of the header */
#define ARCHIVE_CHKSUM_SIZE   8   /* Checksum portion of the header */
#define ARCHIVE_TYPEFLAG_SIZE 1   /* File type portion of the header */
#define ARCHIVE_LINKNAME_SIZE 100 /* Link name portion of the header */
#define ARCHIVE_MAGIC_SIZE    6   /* Magic number portion of the header */
#define ARCHIVE_VERSION_SIZE  2   /* Version portion of the header */
#define ARCHIVE_UNAME_SIZE    32  /* User name portion of the header */
#define ARCHIVE_GNAME_SIZE    32  /* Group name portion of the header */
#define ARCHIVE_DEVMAJOR_SIZE 8   /* Major device number portion of header */
#define ARCHIVE_DEVMINOR_SIZE 8   /* Minor device number portion of header */
#define ARCHIVE_PREFIX_SIZE   155 /* Prefix portion of the header */
#define PATH_MAX              2048 /* Maximum number of characters in a path name */

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
    int verbose, int strict);

/**
 * Lists the contents of a tar archive
 *
 * @param archive_name the name of the archive to list
 * @param verbose a flag to indicate whether to give verbose output while
 listing the archive
 * @param strict a flag to indicate whether to be strict on files conforming to
 the POSIX-specified USTAR archive format
 */
void list_archive(char *archive_name, int verbose, int strict);

/**
 * Extracts the contents of a tar archive
 *
 * @param archive_name the name of the archive to extract
 * @param verbose a flag to indicate whether to give verbose output while
  extracting the archive
 * @param strict a flag to indicate whether to be strict on files conforming to
  the POSIX-specified USTAR archive format
*/
void extract_archive(char *archive_name, int verbose, int strict);
#endif