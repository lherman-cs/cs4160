#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include "net/inaddr/inet_address.h"
#include "net/message/message.h"

int alias_close(int fd);
int alias_connect(int sockfd, const struct sockaddr *address,
                  socklen_t addrlen);
int alias_bind(int sockfd, const struct sockaddr *address, socklen_t addrlen);
int alias_listen(int sockfd, int backlog);
int alias_accept(int sockfd, struct sockaddr *address, socklen_t *addrlen);
ssize_t alias_send(int sockfd, const void *buf, size_t len, int flags);
ssize_t alias_recv(int sockfd, void *buf, size_t len, int flags);

class Socket {
 protected:
  InetAddress addr;
  int type;
  int socket_fd;
  Socket(InetAddress &addr, int type);
  ~Socket();

 public:
  void bind();
  void close();
  virtual Message *recv() = 0;
};