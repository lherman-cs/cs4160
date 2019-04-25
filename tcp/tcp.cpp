#include "tcp.h"
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include "encoder.h"
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
  if (error == -1) {
    if (errno == EINPROGRESS) {
      // /* Wait for connection to complete */
      // fd_set sockets;
      // FD_ZERO(&sockets);
      // FD_SET(sockfd, &sockets);
      // struct timeval timeout = {4, 0};
      // /* You should probably do other work instead of busy waiting on this...
      //    or set a timeout or something */
      // while (select(sockfd + 1, nullptr, &sockets, nullptr, &timeout) <= 0) {
      //   std::cout << "trying to connect " << strerror(errno) << std::endl;
      //   sleep(1);
      // }

    } else {
      throw std::runtime_error("connection failed");
    }
  }

  struct pollfd fd;
  fd.fd = sockfd;
  fd.events = POLLIN;
  return fd;
}

TCP::TCP(const std::string &address) : fd(initSocket(address)) {}

TCP::~TCP() { close(fd.fd); }

bool TCP::connected() {
  int error = 0;
  socklen_t len = sizeof(error);
  int retval = getsockopt(fd.fd, SOL_SOCKET, SO_ERROR, &error, &len);
  std::cout << "[connected]: " << retval << ' ' << strerror(errno) << std::endl;
  std::cout << (errno != EINPROGRESS) << std::endl;
  return errno != EINPROGRESS;
}

bool TCP::read(std::unordered_map<std::string, std::string> &table) {
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

bool TCP::write(const std::unordered_map<std::string, std::string> &resp) {
  if (outPtr == nullptr) {
    auto stream = encode(resp) << '\n';
    out = stream.str();
    outSize = out.size();
    outPtr = out.c_str();
  }

  int length = 0;
  length = send(fd.fd, (void *)outPtr, outSize, 0);
  if (length == -1) {
    if (errno == ENOTCONN) return false;
    throw std::runtime_error(strerror(errno));
  }

  outPtr += length;
  outSize -= length;

  if (outSize > 0) return false;

  outSize = 0;
  outPtr = nullptr;
  return true;
}