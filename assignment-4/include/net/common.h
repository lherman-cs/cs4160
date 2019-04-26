#pragma once
#include <string>
#include <unordered_map>

class Net;

class Common {
 public:
  ~Common();
  Common(const Common&) = delete;
  Common& operator=(const Common&) = delete;
  bool create(const std::string& roomName);
  bool join(const std::string& id);
  bool detail(const std::string& id,
              std::unordered_map<std::string, std::string>& rooms);
  bool list(std::unordered_map<std::string, std::string>& rooms);

 private:
  friend class Net;
  Common();
};