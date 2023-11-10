#ifndef TAR_H
#define TAR_H

/* Begin typedef declarations */
typedef enum FileType FileType;
typedef struct USTARHeader USTARHeader;

/* Represents the type of a file in a tar archive */
enum FileType {
  REGULAR_FILE = '0',
  REGULAR_FILE_ALTERNATE = '\0',
  SYMBOLIC_LINK = '2',
  DIRECTORY = '5'
};

/* Represents a header of the POSIX-specified USTAR archive format */
struct USTARHeader {
  /* File name portion of the header */
  char name[100];
  /* File modes portion of the header */
  char mode[8];
  /* User id portion of the header */
  char uid[8];
  /* Group id portion of the header */
  char gid[8];
  /* File size portion of the header */
  char size[12];
  /* Modification time portion of the header */
  char mtime[12];
  /* Checksum portion of the header */
  char chksum[8];
  /* File type portion of the header */
  FileType typeflag;
  /* Link name portion of the header */
  char linkname[100];
  /* Magic number portion of the header */
  char magic[6];
  /* Version portion of the header */
  char version[2];
  /* User name portion of the header */
  char uname[32];
  /* Group name portion of the header */
  char gname[32];
  /* Major device number portion of header */
  char devmajor[8];
  /* Minor device number portion of header */
  char devminor[8];
  /* Prefix portion of the header */
  char prefix[155];
};

/* Begin function prototype declarations */
void create_archive(char *archive_name, int file_count, char *file_names[],
    int verbose, int strict);
void list_archive(char *archive_name, int verbose, int strict);
void extract_archive(char *archive_name, int verbose, int strict);
#endif /* TAR_H */