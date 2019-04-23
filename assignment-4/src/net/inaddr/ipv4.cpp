
#include "net/inaddr/ipv4.h"
#include "net/utils/utils.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstring>

struct sockaddr_in InetAddressV4::construct_addr(std::string ip,
                                                 uint16_t port) {
  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;

  if (ip == "*") addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Try to parse the dot quad syntax. Else, try to parse the domain name.
  else if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr) <= 0) {
    struct hostent *host = gethostbyname(ip.c_str());

    if (host != NULL) {
      struct in_addr **addr_list = (struct in_addr **)host->h_addr_list;
      addr.sin_addr = *addr_list[0];
    } else
      die(-1, "Invalid IP address/hostname");
  }
  addr.sin_port = htons(port);

  return addr;
}

InetAddressV4::InetAddressV4(std::string ip, uint16_t port)
    : InetAddress(construct_addr(ip, port)) {}
