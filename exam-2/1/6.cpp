#include <algorithm>
#include <iostream>
#include <vector>

class Person {
 public:
  Person(int penis_size) : penis_size(penis_size) {}
  void print() const {
    std::cout << "My penis size is " << penis_size << std::endl;
  }
  static bool penis_sorter(const Person &a, const Person &b) {
    return a.penis_size < b.penis_size;
  }

 private:
  friend class PersonSorter;
  int penis_size;
};

class PersonSorter {
 public:
  bool operator()(const Person &a, const Person &b) {
    return a.penis_size < b.penis_size;
  }
};

int main() {
  std::vector<Person> people;

  people.emplace_back(1000);
  people.emplace_back(2000);
  people.emplace_back(-500);
  people.emplace_back(500);

  std::sort(people.begin(), people.end(), PersonSorter());

  for (const auto &p : people) p.print();
}

void dean_penis_algorithm(std::vector<Person> &p) {
  auto fast = p.begin();
  auto slow = p.begin();
  while (fast != p.end()) {
    fast++;
    fast++;
    slow++;
  }
  p.erase(slow);
}