/* code taken and adapted from http://www.ecst.csuchico.edu/~beej/guide/net/ */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define QUEUE_SIZE 50

int create_service(short port) {
  int fd; /* listen on sock_fd, new connection on new_fd */
  struct sockaddr_in local_addr; /* my address information */
  int yes = 1;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    return -1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    return -1;
  }

  local_addr.sin_family = AF_INET;         /* host byte order */
  local_addr.sin_port = htons(port);       /* short, network byte order */
  local_addr.sin_addr.s_addr = INADDR_ANY; /* automatically fill with my IP */
  memset(&(local_addr.sin_zero), '\0', 8); /* zero the rest of the struct */

  if (bind(fd, (struct sockaddr *)&local_addr, sizeof(struct sockaddr)) == -1) {
    return -1;
  }

  if (listen(fd, QUEUE_SIZE) == -1) {
    return -1;
  }

  return fd;
}

int accept_connection(int fd) {
  int new_fd;
  struct sockaddr_in remote_addr;
  socklen_t size = sizeof(struct sockaddr_in);

  errno = EINTR;
  while (errno == EINTR) {
    if ((new_fd = accept(fd, (struct sockaddr *)&remote_addr, &size)) == -1 &&
        errno != EINTR) {
      return -1;
    } else if (new_fd != -1) {
      break;
    }
  }

  return new_fd;
}
