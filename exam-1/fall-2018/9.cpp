#include <iostream>
#include <list>
const int MAX = 20;
const int MAX_NUMBER = 101;

void eraseLessThan(std::list<int> &l, int max) {
  std::cout << "removing all less than " << max << std::endl;
  auto it = l.begin();
  while (it != l.end()) {
    if (*it < max) {
      it = l.erase(it);
    } else {
      it++;
    }
  }
}

void init(std::list<int> &l) {
  for (auto i = 0; i < MAX; i++) l.emplace_back(rand() % MAX_NUMBER);
}

void print(std::list<int> &l) {
  for (auto el : l) std::cout << el << ' ';
  std::cout << std::endl;
}

int main() {
  std::list<int> l;
  init(l);
  print(l);
  eraseLessThan(l, rand() % 50 + 50 - rand() % 30);
  print(l);
}