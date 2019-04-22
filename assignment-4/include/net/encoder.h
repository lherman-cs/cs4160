#pragma once
#include <string>
#include <unordered_map>

static const char MSGSEP = '\t';
static const char MSGEQ = ':';

char *encode(const std::unordered_map<std::string, std::string> &table);

std::unordered_map<std::string, std::string> decode(const char *);

std::unordered_map<std::string, std::string> decode(const std::string &);
