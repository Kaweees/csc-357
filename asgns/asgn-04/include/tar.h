#ifndef TAR
#define TAR
#define CARCHH
#define REGULAR_FILE '0'
#define SYM_LINK     '2'
#define DIRECTORY    '5'
typedef struct fileHeader {
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char chksum[8];
  char typeflag;
  char linkname[100];
  char magic[6];
  char version[2];
  char uname[32];
  char gname[32];
  char devmajor[8];
  char devminor[8];
  char prefix[155];
  char padding[12];
} fileHeader;
void archiveFile(char *path, FILE *outputFile);
int buildHeaderFromPath(char *path, fileHeader *fh, int sCheck);
int insert_octal(char *where, size_t size, int32_t val);
void writeNamePrefix(fileHeader *fh, char *path);
void traversePath(char *path, FILE *outputFile, int vCheck, int sCheck);
void writeNullBlocks(FILE *outputfile);
void extractArchive(int argc, char *argv[], int vCheck, int sCheck);
char *getName(fileHeader *fH);
void extractFile(fileHeader *fH, char *name, FILE *archive_file, int vCheck);
void regularExtract(fileHeader *fH, char *name, FILE *archive_file);
void directoryExtract(fileHeader *fH, char *name);
void linkExtract(fileHeader *fH, char *name);
void listArchive(int argc, char *argv[], int vCheck, int sCheck);
void writeHeader(fileHeader *fH);
void printName(fileHeader *fH);
void makePermissions(fileHeader *fH, char *permissions);
void checkChksum(fileHeader *fH);
int checkNullBlock(fileHeader *fH);
#endif