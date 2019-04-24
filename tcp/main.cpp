#include <unistd.h>
#include <cassert>
#include <iostream>
#include "tcp.h"

int main() {
#ifdef __EMSCRIPTEN__
  TCP tcp("127.0.0.1:8080");
#else
  TCP tcp("127.0.0.1:8081");
#endif
  std::unordered_map<std::string, std::string> in;
  std::unordered_map<std::string, std::string> out;
  out["command"] = "create";
  out["name"] = "lukas";
  while (!tcp.write(out)) {
    std::cout << "waiting..." << std::endl;
    sleep(1);
  }

  while (1) {
    if (tcp.read(in)) {
      for (auto it : in) {
        std::cout << it.first << ": " << it.second << std::endl;
      }
    }
    sleep(1);
  }
}