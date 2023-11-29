#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>
#include <arpa/inet.h>
#include "tar.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#define BLOCK_SIZE_BYTES 512
#define BUFFER_BLOCK_SIZE 4096
#define BLOCK_SIZE 512
#define PATH_SIZE 256
#define NAME_SIZE 100
#define PREFIX_SIZE 155
#define OCTAL 8
#define BUFFER_BLOCK_SIZE 4096
#define BLOCK_SIZE 512
#define PREFIX_SIZE 155
#define ID_MASK 07777777
#define MODE_MASK 07777
#define NAME_SIZE 100
#define OCTAL 8
#define BUFFER_BLOCK_SIZE 4096
#define BLOCK_SIZE 512
#define PATH_SIZE 256
#define NAME_SIZE 100
#define PREFIX_SIZE 155
#define OCTAL 8

/* Used if uid or gid is larger than the specified size */
int insert_octal(char *location, size_t size, int32_t val)
{
    int ret = 0;
    if (val < 0 || (size < sizeof(val)))
    {
        ret++;
    }
    else
    {
        memset(location,0, size);
        *(int32_t *)(location+size-sizeof(val)) = htonl(val);
        *location |=0x80;
    }
    return ret;
}
/*Builds the fileHeader struct from the current path
 *   Each path has its own unique header*/
int buildHeaderFromPath(char *path, fileHeader *fh, int sCheck)
{
    struct stat sb;
    struct passwd *uid = NULL;
    struct group *gid = NULL;
    unsigned char *header_ptr = (unsigned char*)fh;
    int i;
    unsigned int cSum = 0;
    if(lstat(path, &sb) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if(strlen(path) <= NAME_SIZE)
    {
         strncpy(fh -> name, path, NAME_SIZE);
    }
    else
    {
        printf("check\n");
        writeNamePrefix(fh, path);
        printf("prefix: %s\nname: %s\n", fh->prefix, fh->name);
    }
    /*Fills the fields of the header struct with the specified information
 *       that was extracted using the lstat function. */
    sprintf(fh -> mode, "%07o", sb.st_mode & MODE_MASK);
    if(sb.st_uid > ID_MASK)
    {
         if(sCheck == 1)
         {
            perror("Unable to create conforming header.  Skipping.\n");
            return 0;
         }
         insert_octal(fh -> uid, OCTAL, sb.st_uid);
    }
    else
    {
        sprintf(fh -> uid, "%07o", sb.st_uid);
    }
    if(sb.st_gid > ID_MASK)
    {
        insert_octal(fh -> gid, OCTAL, sb.st_gid);
    }
    else
    {
        sprintf(fh -> gid, "%07o", sb.st_gid);
    }
    /*Sets the typeflags of the current path depending on
 *       whether it is a directory, link, or regular file*/
    if(S_ISREG(sb.st_mode))
    {
        fh -> typeflag = REGULAR_FILE;
        sprintf(fh -> size, "%011lo", sb.st_size);
    }
    else if(S_ISDIR(sb.st_mode))
    {
        fh -> typeflag = DIRECTORY;
        strcpy(fh -> size, "00000000000");
    }
    else if(S_ISLNK(sb.st_mode))
    {
        fh -> typeflag = SYM_LINK;
        if((readlink(path, fh -> linkname, sizeof(fh -> linkname))) == -1)
        {
            perror("symbolic link");
        }
        strcpy(fh -> size, "00000000000");
    }
    else
    {
        perror("unknown file type");
    }
    sprintf(fh -> mtime, "%011lo", sb.st_mtime);
    strcpy(fh -> magic, "ustar\0");
    memcpy(fh -> version, "00", 2);
    uid = getpwuid(sb.st_uid);
    gid = getgrgid(sb.st_gid);
    strcpy(fh -> uname, uid -> pw_name);
    strcpy(fh -> gname, gid -> gr_name);
    for(i = 0; i < BLOCK_SIZE; i++)
    {
        cSum += (unsigned char) *header_ptr;
        header_ptr++;
    }
    cSum += ' ' * OCTAL;
    sprintf(fh -> chksum, "%07o", cSum);
    return 1;
}
/*Archives the file, by writing the contents of it
 *   to the body. */
void archiveFile(char *path, FILE *outputFile)
{
    FILE *inputFile;
    char buffer[BUFFER_BLOCK_SIZE] = {0};
    if((inputFile = fopen(path, "rb")) == NULL)
    {
        perror("file error");
        return;
    }
    while(fread(buffer, BLOCK_SIZE, 1, inputFile))
    {
        fwrite(buffer, BLOCK_SIZE, 1, outputFile);
        memset(buffer, 0, BUFFER_BLOCK_SIZE);
    }
    fwrite(buffer, BLOCK_SIZE, 1, outputFile);
    fclose(inputFile);
    return;
}
/*Writes the prefix of the path if neeeded. Only used
 *  if the name is larger than 100, seperates the path from the main*/
void writeNamePrefix(fileHeader *fh, char *path)
{
    int i;
    for(i = strlen(path) - 1; i >= 0; i--)
    {
        if(path[i] == '/')
        {
            break;
        }
    }
    strncpy(fh->prefix, path, i);
    if(i <= PREFIX_SIZE - 1)
    {
        fh->prefix[i] = '\0';
    }
    strncpy(fh->name, path + i + 1, strlen(path) - i);
}

/* given the command line arguments, loops through tarfile
 * and either extracts all files or finds and extracts specific
 * given files
 * */
void extractArchive(int argc, char *argv[], int vCheck, int sCheck)
{
    FILE *archive;
    fileHeader *fH;
    int readChars = 0, i, foundCheck, size, blocksToSkip;
    char *name;
    /*opens tar file */
    archive = fopen(argv[2], "rb");
    /*allocates space for header struct */
    if(!(fH = (fileHeader*)calloc(sizeof(fileHeader), 1)))
    {
	perror("calloc"); 
	exit(EXIT_FAILURE); 
    }
    /*loops through blocks of tar file */
    while((readChars = fread(fH, 1, BLOCK_SIZE, archive)) > 0)
    {
        if(strtoul(fH->chksum, NULL, OCTAL) == 0)
        {
            break;
        }
        /*checks if hit null block */
        if(checkNullBlock(fH) == 1)
        {
            free(fH);
            return;
        }
        /*checks if tarfile corrupted through chksum */
        checkChksum(fH);
        /*error reading archive */
        if(ferror(archive)){
            perror("reading archive\n");
            exit(EXIT_FAILURE);
        }
        /*strict check, checks user id */
        if(sCheck == 1)
        {
            if(fH->uid[0] & (1 << 7))
            {
                free(fH);
                printf("Malformed header found.  Bailing.\n");
                return;
            }
        }
        /* gets whole name of file from header */
        name = getName(fH);
        if(argc <= 3)/*case where no names given, extracts whole tarfile */
        {
            extractFile(fH, name, archive, vCheck);
        }
        else if(argc >= 4)/*names given, searches for names */
        {
            foundCheck = 0;
            for(i = 3; i < argc; i++)
            {
                /* checks if path is part of searched file */
                if((name[strlen(name)-1] == '/' &&
                    strncmp(name,argv[i],strlen(name))== 0) ||
                   (name[strlen(name)-1] != '/' &&
                    strcmp(name, argv[i]) == 0) ||
                   (strncmp(argv[i],name, strlen(argv[i])) == 0))
                {
                    foundCheck = 1;
                    extractFile(fH, name, archive, vCheck);
                }
            }
            /* if file not extracted, seeks rest of data to get to header*/
            if(foundCheck == 0)
            {
                size = strtoul(fH->size, NULL, OCTAL);
                blocksToSkip = (((size / BLOCK_SIZE) +
                                ((size % BLOCK_SIZE) == 0 ? 0:1 ))*BLOCK_SIZE);
                fseek(archive, blocksToSkip, SEEK_CUR);
            }
        }
        free(name);
    }
    free(fH);
    fclose(archive);
}
/* given file header, processes name and prefix and
 * checks each case to return a pointer to a new string
 * of the combined full name
 * */
char* getName(fileHeader *fH)
{
    char *fullName;
    if(fH->prefix[0] == '\0' && fH->name[NAME_SIZE - 1] == '\0')
    {
        if(!(fullName = (char*)malloc(strlen(fH->name)+ 1)))
	{
	    perror("malloc"); 
	    exit(EXIT_FAILURE); 
	}
        strcpy(fullName, fH->name);
    }
    else
    {
        if(fH->prefix[0] == '\0')
        {
            if(!(fullName = (char*)malloc(NAME_SIZE + 1)))
	    {
	        perror("malloc"); 
		exit(EXIT_FAILURE); 
            }
            strncpy(fullName, fH->name, NAME_SIZE);
            fullName[NAME_SIZE] = '\0';
        }
        else
        {
            fullName = (char*)malloc(PATH_SIZE);
            if(fH->prefix[PREFIX_SIZE - 1] != '\0')
            {
                strncpy(fullName, fH->prefix, PREFIX_SIZE);
                fullName[PREFIX_SIZE] = '\0';
            }
            else
            {
                strcpy(fullName, fH->prefix);
            }
            strcat(fullName, "/");
            if(fH->name[NAME_SIZE - 1] == '\0')
            {
                strcat(fullName, fH->name);
            }
            else
            {
                fullName[strlen(fullName) + NAME_SIZE] = '\0';
                strncat(fullName, fH->name, NAME_SIZE);
            }
        }
    }
    return fullName;
}
/*given file name, has certain process for type of file
 * then extracts
 * */
void extractFile(fileHeader *fH, char *name, FILE *archive_file,  int vCheck)
{
    if(vCheck == 1)
    {
        printf("%s\n", name);
    }
    if(fH->typeflag == REGULAR_FILE)
    {
        regularExtract(fH, name, archive_file);
    }
    else if(fH->typeflag == DIRECTORY)
    {
        directoryExtract(fH, name);
    }
    else if(fH->typeflag == SYM_LINK)
    {
        linkExtract(fH, name);
    }
}
/* if regular file, opens file and writes data to opened file */
void regularExtract(fileHeader *fH, char *name, FILE *archive_file)
{
    FILE *oFile;
    char readBlock[BLOCK_SIZE];
    int totalChars = 0, readChars = 0;
    int size;
    size = strtoul(fH->size, NULL, OCTAL);
    if((oFile = fopen(name, "w")) == NULL)
    {
        fprintf(stderr, "can't open %s: %s\n", name, strerror(errno));
        return;
    }
    while(totalChars < size)
    {
        readChars = fread(readBlock, 1, BLOCK_SIZE, archive_file);
        if((size - totalChars) <= BLOCK_SIZE)
        {
            readChars = (size-totalChars);
        }
        else
        {
            readChars = BLOCK_SIZE;
        }
        totalChars += readChars;
        fwrite(readBlock, readChars, 1, oFile);
        memset(readBlock, 0, BLOCK_SIZE);
    }
    chmod(name, strtoul(fH->mode, NULL, OCTAL));
    chown(name, strtoul(fH->uid, NULL, OCTAL), strtoul(fH->gid, NULL, OCTAL));
    fclose(oFile);
}
/* if extracting directory, creates directory from name */
void directoryExtract(fileHeader *fH, char *name)
{
    mkdir(name, strtoul(fH->mode, NULL, OCTAL));
    chmod(name, strtoul(fH->mode, NULL, OCTAL));
    chown(name, strtoul(fH->uid, NULL, OCTAL), strtoul(fH->gid, NULL, OCTAL));
}
/* if extracting link, creates link from linkname to file name */
void linkExtract(fileHeader *fH, char *name)
{
    symlink(fH->linkname, name);
    chmod(name, strtoul(fH->mode, NULL, 8));
    chown(name, strtoul(fH->uid, NULL, 8), strtoul(fH->gid, NULL, 8));
}

/*Lists the archive, and controls the flow of the table of contents
 *   flag specified within the command line arguments. */
void listArchive(int argc, char *argv[], int vCheck, int sCheck)
{
    FILE *archive;
    fileHeader *fH;
    int blocksToSkip;
    int readChars = 0;
    unsigned int size;
    char *name;
    int i;
    /* opens archive file */
    if((archive = fopen(argv[2], "rb")) == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    /* allocates header struct */
    if(!(fH = (fileHeader*)calloc(sizeof(fileHeader), 1)))
    {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    /* loops through archive file */
    while((readChars = fread(fH, 1, BLOCK_SIZE, archive)) > 0)
    {
        if(ferror(archive)){
            perror("reading archive\n");
            exit(EXIT_FAILURE);
        }
        if(checkNullBlock(fH) == 1)
        {
            free(fH);
            return;
        }
        checkChksum(fH);
        if(sCheck == 1)
        {
            if(fH->uid[0] & (1 << 7))
            {
                free(fH);
                printf("Malformed header found.  Bailing.\n");
                return;
            }
        }
        name = getName(fH);
        if(argc == 3)
        {
            if(vCheck == 0)
            {
                printName(fH);
            }
            else if(vCheck == 1)
            {
                writeHeader(fH);
            }
        }
        else
        {
            for(i = 3; i < argc; i++)
            {
                if(strncmp(argv[i],name, strlen(argv[i])) == 0)
                {
                    if(vCheck == 0)
                    {
                        printName(fH);
                    }
                    else if(vCheck == 1)
                    {
                        writeHeader(fH);
                    }
                }
            }
        }
        free(name);
        size = strtoul(fH->size, NULL, OCTAL);
        blocksToSkip = (((size / BLOCK_SIZE) +
                        ((size % BLOCK_SIZE) == 0 ? 0:1 )) * BLOCK_SIZE);
        fseek(archive, blocksToSkip, SEEK_CUR);
    }
    free(fH);
    fclose(archive);
}
/*This function checks the NULL block, if it is or is not NULL*/
int checkNullBlock(fileHeader *fH)
{
    unsigned int sum = 0;
    unsigned char *headerPtr =  (unsigned char*) fH;
    int i;
    for(i = 0; i<BLOCK_SIZE; i++)
    {
        sum += (unsigned char) *headerPtr;
        headerPtr++;
    }
    return (sum == 0);
}
/*This function is used to check the chksum
 *   checking for correctness. */
void checkChksum(fileHeader *fH)
{
    int chkSum = strtoul(fH->chksum, NULL, OCTAL);
    int i;
    unsigned int sum = 0;
    unsigned char *headerPtr =  (unsigned char*) fH;
    for(i = 0; i<BLOCK_SIZE; i++)
    {
        if(i< PREFIX_SIZE - 7 || i > PREFIX_SIZE)
        {
            sum += (unsigned char) *headerPtr;
        }
        headerPtr++;
    }
    sum += ' ' * OCTAL;
    if(chkSum != sum)
    {
        perror("Bad tarfile");
        exit(EXIT_FAILURE);
    }
}
/*This function writes the header given the fileHeader.
 *   Used when the verbose flag is specified */
void writeHeader(fileHeader *fH)
{
    char* permissions = (char*)malloc(11);
    char mtimeString[17];
    time_t mtime = strtoul(fH->mtime, NULL, OCTAL);
    struct tm *time = NULL;
    char *name;
    if(!(name = (char*)malloc(strlen(fH->uname) + strlen(fH->gname) + 2)))
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    /* adds name and groupname to name */
    strcpy(name, fH->uname);
    strcat(name, "/");
    strcat(name, fH->gname);
    /* gets modified time */
    time = localtime(&mtime);
    sprintf(mtimeString, "%04i-%02i-%02i %02i:%02i",
            1900 + time->tm_year,
            time->tm_mon + 1,
            time->tm_mday,
            time->tm_hour,
            time->tm_min);
    makePermissions(fH, permissions);
    printf("%s ", permissions);
    printf("%17s ", name);
    printf("%8i ", (int)strtoul(fH->size, NULL, OCTAL));
    printf("%16s ", mtimeString);
    printName(fH);
    free(permissions);
    free(name);
}
/*Makes the permissions from the fileHeader, used for printing
 *   when the verbose flag is specified*/
void makePermissions(fileHeader *fH, char *permissions)
{
    int mode;
    mode = strtoul(fH->mode, NULL, OCTAL);
    if(fH->typeflag == DIRECTORY)
    {
        permissions[0] = 'd';
    }
    else if(fH->typeflag == SYM_LINK)
    {
        permissions[0] = 'l';
    }
    else
    {
        permissions[0] = '-';
    }
    permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';
}
/*Used to print the names, extra functionality is added
 *   to make sure that the prefix and the name is correctly
 *     concatenated and added. */
void printName(fileHeader *fH)
{
    char *fullName;
    if(fH->prefix[0] == '\0' && fH->name[NAME_SIZE - 1] == '\0')
    {
        printf("%s\n", fH->name);
        return;
    }
    else
    {
        if(fH->prefix[0] == '\0')
        {
            if(!(fullName = (char*)malloc(NAME_SIZE + 1)))
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            strncpy(fullName, fH->name, NAME_SIZE);
            fullName[NAME_SIZE] = '\0';
        }
        else
        {
            if(!(fullName = (char*)malloc(PATH_SIZE)))
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            if(fH->prefix[PREFIX_SIZE - 1] != '\0')
            {
                strncpy(fullName, fH->prefix, PREFIX_SIZE);
                fullName[PREFIX_SIZE] = '\0';
            }
            else
            {
                strcpy(fullName, fH->prefix);
            }
            strcat(fullName, "/");
            if(fH->name[NAME_SIZE - 1] == '\0')
            {
                strcat(fullName, fH->name);
            }
            else
            {
                fullName[strlen(fullName) + NAME_SIZE] = '\0';
                strncat(fullName, fH->name, NAME_SIZE);
            }
        }
        printf("%s\n", fullName);
        free(fullName);
    }
}

/*Traverses the current path, and builds the headers
 *   as well as controls the flow of archiving the files. */

void traversePath(char *path, FILE *outputFile, int vCheck, int sCheck)
{
    struct stat sb;
    struct dirent *d;
    DIR *dir;
    char *str, *tempPath;
    struct fileHeader *tempFh;
    if(!(tempFh = (fileHeader*)calloc(sizeof(fileHeader),1)))
    {
	perror("calloc"); 
	exit(EXIT_FAILURE); 
    }
    if((lstat(path, &sb)) == -1)
    {
        perror("stat\n");
        return;
    }
    /*Check for the verbose flag*/
    if(vCheck == 1)
    {
        printf("%s\n", path);
    }
    /*If the file specified is of type regular file*/
    if(S_ISREG(sb.st_mode))
    {
        if(buildHeaderFromPath(path, tempFh, sCheck) == 0)
        {
            free(tempFh);
            return;
        }
        fwrite(tempFh, sizeof(fileHeader), 1, outputFile);
        if(sb.st_size != 0)
        {
            archiveFile(path, outputFile);
        }
    }
    /*If the file specified is of type symbolic link*/
    else if(S_ISLNK(sb.st_mode))
    {
        if(buildHeaderFromPath(path, tempFh, sCheck) == 0)
        {
            free(tempFh);
            return;
        }
        fwrite(tempFh, sizeof(fileHeader), 1, outputFile);
    }
    /*If the file specified is of type directory*/
    else if(S_ISDIR(sb.st_mode))
    {
        if((dir = opendir(path)) == NULL)
        {
            return;
        }
        /*If the path of the file does not end with a "/", append a "/"*/
        if(path[strlen(path)-1] != '/')
        {
            if(!(tempPath = (char *)calloc(strlen(path)+2, sizeof(char))))
	    {
		perror("calloc"); 
		exit(EXIT_FAILURE); 
            }
            strcpy(tempPath, path);
            strcat(tempPath, "/");
        }
        else
        {
            if(!(tempPath = (char *)calloc(strlen(path) + 1, sizeof(char))))
	    {
		perror("calloc"); 
		exit(EXIT_FAILURE); 
	    }
            strcpy(tempPath, path);
        }
        if(buildHeaderFromPath(tempPath, tempFh, sCheck) == 0)
        {
            free(tempFh);
            closedir(dir);
            return;
        }
        fwrite(tempFh, sizeof(fileHeader), 1, outputFile);
        /*Makes it so that the recursion of the file system does not
 *           recurse upon the "." and the ".." directories otherwise it causes
 *                     an infinite loop */
        while((d = readdir(dir)) != NULL)
        {
            if(strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0)
            {
                if(!(str = (char*)calloc
                  (strlen(tempPath)+ strlen(d->d_name) + 1,1)))
		{
		    perror("calloc"); 
		    exit(EXIT_FAILURE); 
		}
                strcpy(str, tempPath);
                strcat(str, d->d_name);
                traversePath(str, outputFile, vCheck, sCheck);
                free(str);
            }
        }
        free(tempPath);
        closedir(dir);
    }
    else
    {
        printf("%s not regular file or directory\n", path);
    }
    free(tempFh);
}
/*Function to write the final two null blocks to the end of the
 *   archive signifying the end. */
void writeNullBlocks(FILE *outputfile)
{
    char nullBlock[BLOCK_SIZE_BYTES * 2] = {0};
    fwrite(nullBlock, BLOCK_SIZE_BYTES * 2, 1, outputfile);
}
