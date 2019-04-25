#include "net/common.h"
#include <unordered_map>
#include "net/encoder.h"
#include "net/tcp.h"

void Common::create(const std::string gameName) {
  std::unordered_map<std::string, std::string> msg;
  msg["command"] = "create";
  msg["name"] = gameName;
  TCPSender s;
  s.write(encode(msg));
}

void Messenger::join() {
  if (gameID == "") {
    std::cerr << "Game has not been properly initialized, please wait for a "
                 "response from the server"
              << std::endl;
    return;
  }
  std::unordered_map<std::string, std::string> msg;
  msg["command"] = "join";
  msg["id"] = gameID;
  TCPSender s;
  s.write(encode(msg));
}

void Messenger::detail() {
  if (gameID == "") {
    std::cerr << "Game has not been properly initialized, please wait for a "
                 "response from the server"
              << std::endl;
    return;
  }
  std::unordered_map<std::string, std::string> msg;
  msg["command"] = "detail";
  msg["id"] = gameID;
  TCPSender s;
  s.write(encode(msg));
}

void Messenger::list() {
  if (gameID == "") {
    std::cerr << "Game has not been properly initialized, please wait for a "
                 "response from the server"
              << std::endl;
    return;
  }
  std::unordered_map<std::string, std::string> msg;
  msg["command"] = "list";
  msg["id"] = gameID;
  TCPSender s;
  s.write(encode(msg));
}