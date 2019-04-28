#pragma once
#include <string>
#include <unordered_map>
#include <vector>

static const char MSGSEP = '\t';
static const char MSGEQ = ':';

std::stringstream encode(
    const std::unordered_map<std::string, std::string> &table);

void decode(const std::string &msg,
            std::unordered_map<std::string, std::string> &table);

const std::vector<std::string> tostr(const std::string &in);

const std::vector<int> toint(const std::string &in);