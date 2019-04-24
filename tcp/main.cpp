#include <unistd.h>
#include <cassert>
#include <iostream>
#include "tcp.h"

int main() {
  TCP tcp("127.0.0.1:8080");
  std::unordered_map<std::string, std::string> table;

  while (!tcp.read(table)) {
    std::cout << "waiting..." << std::endl;
    sleep(1);
  }

  for (auto it : table) {
    std::cout << it.first << ": " << it.second << std::endl;
  }
}