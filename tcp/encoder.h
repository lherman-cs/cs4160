#pragma once
#include <string>
#include <unordered_map>

static const char MSGSEP = '\t';
static const char MSGEQ = ':';

std::string encode(const std::unordered_map<std::string, std::string> &table);

void decode(const std::string &msg,
            std::unordered_map<std::string, std::string> &table);
