#pragma once
#include "inet_address.h"

// Note: InetAddressV6 doesn't support hostnames. Please use InetAddressV4 if
//       you want to use hostnames
class InetAddressV6 : public InetAddress {
 private:
  struct sockaddr_in6 construct_addr(std::string ip, uint16_t port);

 public:
  // If "ip" = "*", use any ip address from the network interface
  InetAddressV6(std::string ip, uint16_t port);
};