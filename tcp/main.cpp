#include <unistd.h>
#include <cassert>
#include <iostream>
#include "tcp.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

TCP *tcp = nullptr;
bool receiving = false;

void loop() {
  if (!receiving) {
    std::unordered_map<std::string, std::string> out;
    out["command"] = "create";
    out["name"] = "lukas";
    if (tcp->write(out)) {
      receiving = true;
    }
  } else {
    std::unordered_map<std::string, std::string> in;
    if (tcp->read(in)) {
      for (auto it : in) {
        std::cout << it.first << ": " << it.second << std::endl;
      }
    }
  }
}

int main() {
  try {
#ifdef __EMSCRIPTEN__
    TCP _tcp("127.0.0.1:8080");
#else
    TCP _tcp("127.0.0.1:8081");
#endif
    tcp = &_tcp;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 1, 1);
#else
    while (1) {
      loop();
      sleep(1);
    }
#endif
  } catch (std::runtime_error err) {
    std::cout << err.what() << std::endl;
  }
}