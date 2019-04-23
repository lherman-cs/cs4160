#include "net/inaddr/inet_address.h"
#include "net/utils/utils.h"

#include <sys/socket.h>
#include <cstring>

InetAddress::InetAddress(const struct sockaddr_in& addr4) {
  this->_addr.addr4 = addr4;
  this->size = sizeof(addr4);
}

InetAddress::InetAddress(const struct sockaddr_in6& addr6) {
  this->_addr.addr6 = addr6;
  this->size = sizeof(addr6);
}

InetAddress::InetAddress(const struct sockaddr* addr, socklen_t addrlen) {
  memset(&this->_addr, 0, sizeof(this->_addr));
  memcpy(&this->_addr, addr, addrlen);
  this->size = addrlen;
}

InetAddress::InetAddress(const InetAddress& other) {
  this->_addr = other._addr;
  this->size = other.size;
}

sa_family_t InetAddress::get_family() { return this->_addr.addr.sa_family; }

struct sockaddr* InetAddress::get_addr() {
  return &this->_addr.addr;
}

socklen_t InetAddress::get_size() { return this->size; }
