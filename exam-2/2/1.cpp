#include <functional>
#include <iostream>

namespace Functions {
using f = void(const std::string&, unsigned int);
};

int main() {
  std::function<Functions::f> back;
  back = [&back](const std::string& s, unsigned int index) {
    if (index == s.size()) return;
    back(s, index + 1);
    std::cout << s[index];
  };
  back("front", 0);
  std::cout << std::endl;
}