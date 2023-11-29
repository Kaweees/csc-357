#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "create_archive.h"
#include "traverse_directory.h"
#define BLOCK_SIZE_BYTES 512
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
