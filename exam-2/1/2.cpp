#include <iostream>

int main() {
  char ch;
  std::cin >> ch;
  std::cout << ch << std::endl;

  auto isletter = [](char ch) { return (ch >= 'A' && ch <= 'Z'); };
  std::cout << "is: " << isletter(ch) << std::endl;
}