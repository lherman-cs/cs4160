#include <iostream>
#include <string>

class Bird {
 public:
  virtual ~Bird() {}
  Bird(const std::string& s) : species(s) {}
  virtual void swim() const { std::cout << "I can't swim" << std::endl; }

 private:
  std::string species;
};

class Penguin : public Bird {
 public:
  Penguin(const std::string& species) : Bird(species) {}
  void swim() const { std::cout << "I can swim" << std::endl; }
};

int main() {
  Bird* bird = new Penguin("penguin");
  bird->swim();
}