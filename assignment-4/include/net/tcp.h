// singleton?
#pragma once
#include "core/interface.h"
#include "net/inaddr/ipv4.h"
#include "net/socket/tcp.h"

class TCP : public Reader, public Writer {
 public:
  virtual ~TCP();
  TCP();
  virtual char* read();
  virtual void write(const char*);

 private:
  InetAddressV4 addr{"localhost", 8004};
  TCPSocket sock{addr};
}