#include <unistd.h>
#include <cassert>
#include <iostream>
#include "tcp.h"

void read(TCP &tcp) {
  std::unordered_map<std::string, std::string> table;
  while (!tcp.read(table)) {
    std::cout << "waiting..." << std::endl;
    sleep(1);
  }

  for (auto it : table) {
    std::cout << it.first << ": " << it.second << std::endl;
  }
}

void write(TCP &tcp, std::unordered_map<std::string, std::string> &table) {
  while (!tcp.write(table)) {
    std::cout << "waiting..." << std::endl;
    sleep(1);
  }
}

int main() {
  TCP tcp("127.0.0.1:8080");
  std::unordered_map<std::string, std::string> table;
  table["command"] = "create-room";
  table["name"] = "lukas";

  write(tcp, table);
  table.clear();
  read(tcp);
}