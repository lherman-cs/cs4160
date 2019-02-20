#include <iostream>
#include <vector>

class Console {
 public:
  Console() { std::cout << "default" << std::endl; }
  Console(const char* name) : name(name) {
    std::cout << name << " convert" << std::endl;
  }
  Console(const Console& other) {
    name = other.name;
    std::cout << name << " copy" << std::endl;
  }
  ~Console() { std::cout << name << " destructor" << std::endl; }

 private:
  const char* name;
};

int main() {
  std::vector<Console> games;
  games.push_back("Switch");
  games.emplace_back("PS4");
}