#pragma once
#include "net/socket/socket.h"

class UDPSocket : public Socket {
 public:
  UDPSocket(InetAddress &addr);
  ~UDPSocket();
  void listen_and_serve(void (*handler)(UDPSocket *socket,
                                        InetAddress client_addr));
  void serve(void (*handler)(UDPSocket *socket, InetAddress server_addr));
  void send(InetAddress *to_addr, Message *msg);
  Message *recv();
};