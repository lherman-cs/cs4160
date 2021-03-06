#include "net/encoder.h"
#include <sstream>
#include <string>
#include <vector>

// encode
std::stringstream encode(
    const std::unordered_map<std::string, std::string> &table) {
  std::stringstream ss;
  for (const auto &it : table) ss << it.first << MSGEQ << it.second << MSGSEP;
  ss << '\n';
  return ss;
}

void decode(const std::string &msg,
            std::unordered_map<std::string, std::string> &table) {
  std::string key, val;
  std::istringstream iss(msg);

  while (std::getline(std::getline(iss, key, MSGEQ), val, MSGSEP))
    table[key] = val;
}

// parses a comma-seperated string into array of strings
const std::vector<std::string> toVecStr(const std::string &in) {
  if (in.length() == 0) return {};
  std::stringstream ss(in);
  std::vector<std::string> result;

  while (ss.good()) {
    std::string substr;
    getline(ss, substr, ',');
    result.push_back(substr);
  }
  return result;
}

// parses a comma-seperated string into array on ints
const std::vector<int> toVecInt(const std::string &in) {
  if (in.length() == 0) return {};
  std::stringstream ss(in);
  std::vector<int> result;

  while (ss.good()) {
    std::string substr;
    getline(ss, substr, ',');
    result.push_back(std::stoi(substr));
  }

  return result;
}