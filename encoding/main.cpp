#include <cassert>
#include <iostream>
#include "message.h"

void display(std::unordered_map<std::string, std::string>& table) {}

int main() {
  std::unordered_map<std::string, std::string> table;
  table["name"] = "Lukas";
  table["age"] = "23";

  auto out = encode(table);
  assert(out == "age:23\rname:Lukas\r\n");

  auto table_2 = decode(out);
  for (const auto& it : table) {
    assert(it.second == table_2[it.first]);
  }

  assert(table_2.size() == table.size());
}