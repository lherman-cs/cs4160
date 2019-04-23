// singleton?

#include <SDL2/SDL_net.h>
#include "core/interface.h"

class TCP : public Reader, public Writer {
 public:
  virtual ~TCP() {}
  TCP() {}
  virtual char* read() const;
  virtual void write(const char*) const;

 private:
  IPaddress ip;
  TCPsocket sock;
  Uint16 port;
}