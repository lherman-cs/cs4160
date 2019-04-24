#pragma once

#include <poll.h>
#include <sstream>
#include <string>
#include <unordered_map>

class TCP {
 public:
  TCP(const std::string &address);
  ~TCP();
  // return true, if it's accepted message.
  bool read(std::unordered_map<std::string, std::string> &table);
  bool write(const std::unordered_map<std::string, std::string> &resp);

 private:
  struct pollfd fd;
  const int timeout = 1000;  // in miliseconds
  std::stringstream in;
  std::string out;
  const char *outPtr = nullptr;
  size_t outSize = 0;
  // static const size_t pollfdSize = sizeof(struct pollfd);
};