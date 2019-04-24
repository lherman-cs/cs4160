#include "encoder.h"
#include <sstream>

// encode
std::string encode(const std::unordered_map<std::string, std::string> &table) {
  std::stringstream ss;
  for (const auto &it : table) ss << it.first << MSGEQ << it.second << MSGSEP;
  return ss.str();
}

void decode(const std::string &msg,
            std::unordered_map<std::string, std::string> &table) {
  std::string key, val;
  std::istringstream iss(msg);

  while (std::getline(std::getline(iss, key, MSGEQ), val, MSGSEP))
    table[key] = val;
}