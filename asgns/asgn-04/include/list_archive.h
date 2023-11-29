#ifndef LISTH
#define LISTH
void listArchive(int argc, char *argv[], int vCheck, int sCheck);
void writeHeader(fileHeader *fH);
void printName(fileHeader *fH);
void makePermissions(fileHeader *fH, char *permissions);
void checkChksum(fileHeader *fH);
int checkNullBlock(fileHeader *fH);
#endif
