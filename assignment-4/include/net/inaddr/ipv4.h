#ifndef IPV4_HPP
#define IPV4_HPP

#include "inet_address.h"

class InetAddressV4 : public InetAddress {
 private:
  struct sockaddr_in construct_addr(std::string ip, uint16_t port);

 public:
  // If "ip" = "*", use any ip address from the network interface
  InetAddressV4(std::string ip, uint16_t port);
};

#endif