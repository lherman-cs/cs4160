#include "net/inaddr/ipv6.h"
#include "net/utils/utils.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstring>

struct sockaddr_in6 InetAddressV6::construct_addr(std::string ip,
                                                  uint16_t port) {
  struct sockaddr_in6 addr;

  memset(&addr, 0, sizeof(addr));
  addr.sin6_family = AF_INET6;

  if (ip == "*")
    addr.sin6_addr = in6addr_any;
  else {
    // Parse ipv6 string and store in addr.sin_addr
    int rc = inet_pton(AF_INET6, ip.c_str(), &addr.sin6_addr);
    if (rc <= 0) die(rc, "Invalid IP address/hostname");
  }

  addr.sin6_port = htons(port);
  return addr;
}

InetAddressV6::InetAddressV6(std::string ip, uint16_t port)
    : InetAddress(construct_addr(ip, port)) {}
