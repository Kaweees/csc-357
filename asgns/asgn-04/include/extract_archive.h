#ifndef EXTRACTH
#define EXTRACTH
void extractArchive(int argc, char *argv[], int vCheck, int sCheck);
char* getName(fileHeader *fH);
void extractFile(fileHeader *fH, char *name, FILE *archive_file,  int vCheck);
void regularExtract(fileHeader *fH, char *name, FILE *archive_file);
void directoryExtract(fileHeader *fH, char *name);
void linkExtract(fileHeader *fH, char *name);
#endif
