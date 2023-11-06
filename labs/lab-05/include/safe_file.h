#include <stdlib.h>
#include <unistd.h>

#ifndef SAFE_FILE_H
#define SAFE_FILE_H

typedef struct FileContent FileContent;
typedef struct FileStat FileStat;

/* Represents the contents of a file */
struct FileContent {
  /* The length of the file contents in bytes */
  ssize_t file_size;
  /* The pointer to the file contents */
  unsigned char *file_contents;
};

/* Represents the metadata of a file */
struct FileStat {
  /* ID of device containing file */
  dev_t     st_dev;
  /* inode number */
  ino_t     st_ino; 
  /* protection */    
  mode_t    st_mode;    
  /* number of hard links */
  nlink_t   st_nlink;   
  /* user ID of owner */
  uid_t     st_uid;     
  /* group ID of owner */
  gid_t     st_gid;     
  /* device ID (if special file) */
  dev_t     st_rdev;    
  /* total size, in bytes */
  off_t     st_size;    
  /* blocksize for file system I/O */
  blksize_t st_blksize; 
  /* number of 512B blocks allocated */
  blkcnt_t  st_blocks;  
  /* time of last access */
  time_t    st_atime;   
  /* time of last modification */
  time_t    st_mtime;   
  /* time of last status change */
  time_t    st_ctime;   
};

int safeOpen(char *filename, int flags, mode_t mode);
FileContent *safeRead(int fd);
void safeWrite(int fd, void *buf, size_t count);
void safeClose(int fd);
void freeFileContent(FileContent *file_contents);
#endif