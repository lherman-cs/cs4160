#pragma once

#include <string>
#include <unordered_map>

// encode
std::string encode(const std::unordered_map<std::string, std::string> &table);

// decode
std::unordered_map<std::string, std::string> decode(const std::string &);