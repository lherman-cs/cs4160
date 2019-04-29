#pragma once

#include <poll.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include "global/global.h"

#ifdef DEBUG
#ifdef __EMSCRIPTEN__
static const std::string endpoint =
    Global::get().gamedata.getXmlStr("net/dev/wasm");
#else
static const std::string endpoint =
    Global::get().gamedata.getXmlStr("net/dev/native");
#endif
#else
#ifdef __EMSCRIPTEN__
static const std::string endpoint =
    Global::get().gamedata.getXmlStr("net/prod/wasm");
#else
static const std::string endpoint =
    Global::get().gamedata.getXmlStr("net/prod/native");
#endif
#endif

namespace net {
using message = std::unordered_map<std::string, std::string>;
inline std::shared_ptr<message> create(const std::string &name) {
  auto req = std::make_shared<message>();
  (*req)["command"] = "create";
  (*req)["name"] = name;
  return req;
}

inline std::shared_ptr<message> join(const std::string &id) {
  auto req = std::make_shared<message>();
  (*req)["command"] = "join";
  (*req)["id"] = id;
  return req;
}

inline std::shared_ptr<message> subscribe() {
  auto req = std::make_shared<message>();
  (*req)["command"] = "subscribe";
  return req;
}

inline std::shared_ptr<message> gameStart() {
  auto req = std::make_shared<message>();
  (*req)["command"] = "game-start";
  return req;
}
}  // namespace net

class TCP {
 public:
  TCP(const std::string &address = endpoint);
  ~TCP();
  TCP(const TCP &) = delete;
  TCP &operator=(const TCP &) = delete;

  // return true, if it's accepted message.
  bool read(net::message &table);
  bool write(const net::message &resp);
  inline bool isOffline() const { return offline; };
  inline std::string getError() const { return error; };

 private:
  void panic(const std::string &error);
  struct pollfd fd;
  bool offline = false;
  std::string error = "";
  const int timeout = 100;  // in miliseconds
  std::stringstream in{};
  std::string out{};
  const char *outPtr = nullptr;
  size_t outSize = 0;
};