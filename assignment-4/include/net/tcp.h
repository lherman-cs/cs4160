#pragma once

#include <poll.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include "global/global.h"

#ifdef __EMSCRIPTEN__
static const std::string endpoint =
    Global::get().gamedata.getXmlStr("net/wasm");
#else
static const std::string endpoint =
    Global::get().gamedata.getXmlStr("net/native");
#endif

class TCP {
 public:
  TCP(const std::string &address = endpoint);
  ~TCP();
  TCP(const TCP &) = delete;
  TCP &operator=(const TCP &) = delete;

  // return true, if it's accepted message.
  bool read(std::unordered_map<std::string, std::string> &table);
  bool write(const std::unordered_map<std::string, std::string> &resp);

 private:
  struct pollfd fd;
  const int timeout = 100;  // in miliseconds
  std::stringstream in;
  std::string out;
  const char *outPtr = nullptr;
  size_t outSize = 0;
};