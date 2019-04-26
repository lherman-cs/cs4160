#include <iostream>
#include <map>
#include <string>

void display(const std::map<std::string, int>& pokemon) {
  for (auto it : pokemon) {
    std::cout << it.first << ", " << it.second << std::endl;
  }
}

int main() {
  std::map<std::string, int> pokemon;
  pokemon["Noctis"] = 2750;
  pokemon["Pronto"] = 1725;
  pokemon["Noctis"] = 1750;
  std::cout << pokemon.size() << std::endl;
  display(pokemon);
}