#pragma once
#include <string>
#include "core/observable.h"

class Player : public Observable {
 protected:
  Player(const std::string &name);

 public:
  virtual ~Player();

 protected:
  std::string name;
};