#ifndef CARCHH
#define CARCHH
#define REGULAR_FILE '0'
#define SYM_LINK '2'
#define DIRECTORY '5'
typedef struct fileHeader{
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
}fileHeader;
void archiveFile(char *path, FILE *outputFile);
int buildHeaderFromPath(char *path, fileHeader *fh, int sCheck);
int insert_octal(char *where, size_t size, int32_t val);
void writeNamePrefix(fileHeader *fh, char *path);
#endif
