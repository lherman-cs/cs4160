#include "message.h"
#include <sstream>

static const char MSGSEP = '\r';
static const char MSGEND = '\n';
static const char MSGEQ = ':';

// encode
std::string encode(const std::unordered_map<std::string, std::string> &table) {
  std::stringstream ss;
  for (const auto &it : table) ss << it.first << MSGEQ << it.second << MSGSEP;
  ss << MSGEND;
  return ss.str();
}

// decode
std::unordered_map<std::string, std::string> decode(const std::string &msg) {
  std::unordered_map<std::string, std::string> table;

  std::string key, val;
  std::istringstream iss(msg);

  while (std::getline(std::getline(iss, key, MSGEQ), val, MSGSEP))
    table[key] = val;

  return table;
}