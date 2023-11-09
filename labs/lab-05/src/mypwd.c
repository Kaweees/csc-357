#include <arpa/inet.h>
#include <fcntl.h>
#include <libgen.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "linked_list.h"
#include "safe_alloc.h"
#include "safe_dir.h"
#include "safe_file.h"

void mypwd() {
  LinkedList* ll = createLinkedList();
  char* dir_path = safeCalloc(PATH_MAX, sizeof(char));
  struct stat* cwd_stat = safeMalloc(sizeof(struct stat));
  struct stat* par_stat = safeMalloc(sizeof(struct stat));
  do {
    safeLstat(".", cwd_stat);
    safeChdir("..");
    safeLstat(".", par_stat);
    if (cwd_stat->st_dev == par_stat->st_dev &&
        cwd_stat->st_ino == par_stat->st_ino) {
      break;
    } else {
      Node* node = createNode(cwd_stat->st_dev, cwd_stat->st_ino);
      insertNode(ll, node);
    }
  } while (1);
  if (ll->size == 0) {
    strcat(dir_path, "/");
  } else {
  }
  while (ll->size > 0) {
    Node* node = removeFirstNode(ll);
    DIR* cwd = safeOpenDir(".");
    DirContent* cwd_contents = safeReadDir(cwd);
    size_t i;
    for (i = 0; i < cwd_contents->num_entries; i++) {
      struct dirent* cwd_entry = cwd_contents->entries[i];
      struct stat* stat = safeMalloc(sizeof(struct stat));
      safeLstat(cwd_entry->d_name, stat);
      if (node->ino == stat->st_ino && node->dev == stat->st_dev) {
        strcat(dir_path, "/");
        strcat(dir_path, cwd_entry->d_name);
        safeChdir(cwd_entry->d_name);
        safeFree(stat);
        break;
      }
      safeFree(stat);
    }
    freeDirContent(cwd_contents);
    safeFree(node);
    safeCloseDir(cwd);
  }
  printf("%s\n", dir_path);
  safeFree(dir_path);
  safeFree(ll);
  safeFree(cwd_stat);
  safeFree(par_stat);
}

int main(int argc, char* argv[]) {
  mypwd();
  return EXIT_SUCCESS;
}