#include "net/socket/socket.h"
#include "net/utils/utils.h"

#include <sys/socket.h>
#include <unistd.h>

int alias_close(int fd) { return close(fd); }
int alias_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  return connect(sockfd, addr, addrlen);
}

int alias_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  return bind(sockfd, addr, addrlen);
}

int alias_listen(int sockfd, int backlog) { return listen(sockfd, backlog); }

int alias_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
  return accept(sockfd, addr, addrlen);
}

ssize_t alias_send(int sockfd, const void *buf, size_t len, int flags) {
  return send(sockfd, buf, len, flags);
}

ssize_t alias_recv(int sockfd, void *buf, size_t len, int flags) {
  return recv(sockfd, buf, len, flags);
}

Socket::Socket(InetAddress &addr, int type) : addr(addr) {
  this->type = type;
  this->socket_fd = socket(addr.get_family(), this->type, 0);
  if (this->socket_fd == -1) die(-1, "Socket file descriptor error");
}

Socket::~Socket() { alias_close(this->socket_fd); }

void Socket::bind() {
  int rc =
      alias_bind(this->socket_fd, this->addr.get_addr(), this->addr.get_size());

  if (rc < 0) die(rc, "Can't bind");
}

void Socket::close() { alias_close(this->socket_fd); }