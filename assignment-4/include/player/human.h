#pragma once
#include "player/player.h"

class Human : public Player {
 public:
  Human(const std::string &name);
  ~Human() {}
};