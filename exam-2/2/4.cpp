// find_if_not example
#include <algorithm>  // std::find_if_not
#include <array>      // std::array
#include <iostream>   // std::cout

int main() {
  std::array<int, 5> foo = {1, 2, 3, 4, 5};

  std::array<int, 5>::iterator it =
      std::find_if(foo.begin(), foo.end(), [](int i) { return i % 2 == 0; });
  std::cout << "The first even value is " << *it << '\n';

  return 0;
}