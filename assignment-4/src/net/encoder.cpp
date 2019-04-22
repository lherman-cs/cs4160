#include "net/encoder.h"
#include <sstream>

// encode
char *encode(const std::unordered_map<std::string, std::string> &table) {
  std::stringstream ss;
  for (const auto &it : table) ss << it.first << MSGEQ << it.second << MSGSEP;
  return ss.str().data();
}

// decode
std::unordered_map<std::string, std::string> decode(const char *msg) {
  std::unordered_map<std::string, std::string> table;

  std::string key, val;
  std::istringstream iss(msg);

  while (std::getline(std::getline(iss, key, MSGEQ), val, MSGSEP))
    table[key] = val;

  return table;
}

std::unordered_map<std::string, std::string> decode(const std::string &msg) {
  return decode(msg.data());
}