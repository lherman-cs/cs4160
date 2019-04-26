#include "net/tcp.h"
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include "net/encoder.h"
#define BUFFSIZE 1024

static struct pollfd initSocket(const std::string &address) {
  std::string _address = address;
  std::replace(_address.begin(), _address.end(), ':', ' ');
  std::stringstream ss(_address);

  std::string host;
  std::string port;

  if (!(ss >> host)) {
    throw std::runtime_error("address is <host>:<port>");
  }

  if (!(ss >> port)) {
    throw std::runtime_error("address is <host>:<port>");
  }

  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  int error = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
  if (error) {
    throw std::runtime_error(gai_strerror(error));
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd == -1) {
    throw std::runtime_error("failed to create socket");
  }

  fcntl(sockfd, F_SETFL, O_NONBLOCK);
  error = connect(sockfd, res->ai_addr, res->ai_addrlen);
  freeaddrinfo(res);
  if (error == -1 && errno != EINPROGRESS) {
    throw std::runtime_error("connection failed");
  }

  struct pollfd fd;
  fd.fd = sockfd;
  fd.events = POLLIN;
  return fd;
}

TCP::TCP(const std::string &address) : fd(initSocket(address)) {}

TCP::~TCP() { close(fd.fd); }

bool TCP::read(net::message &table) {
  int rv = poll(&fd, 1, timeout);
  if (rv == -1) {
    throw std::runtime_error(strerror(errno));
  }

  // timeout
  if (rv == 0 && !(fd.revents & POLLIN)) return false;

  char buf;
  bool splitted = false;
  while (recv(fd.fd, &buf, 1, 0) > 0) {
    if (buf == '\n') {
      splitted = true;
      break;
    }

    in << buf;
  }

  if (!splitted) return false;

  decode(in.str(), table);
  in.str("");
  return true;
}

bool TCP::write(const net::message &resp) {
  if (outPtr == nullptr) {
    auto stream = encode(resp);
    out = stream.str();
    outSize = out.size();
    outPtr = out.c_str();
  }

  int length = 0;
  length = send(fd.fd, reinterpret_cast<const void *>(outPtr), outSize, 0);
  if (length == -1) {
    if (errno == ENOTCONN || errno == EAGAIN) return false;
    throw std::runtime_error(strerror(errno));
  }

  if (length == 0 && errno == EPIPE) {
    return false;
  }

  outPtr += length;
  outSize -= static_cast<size_t>(length);

  if (outSize > 0) return false;

  outSize = 0;
  outPtr = nullptr;
  return true;
}