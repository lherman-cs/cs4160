#pragma once
#include <string>
#include "core/observable.h"
#include "screens/game.h"

class Player : public Observable {
 public:
  virtual ~Player();
  const std::string name;

 protected:
  Player(const GameScreen &game, const std::string &name);
  const GameScreen &game;
};