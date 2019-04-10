#pragma once
#include <SDL2/SDL.h>
#include "player.h"

class Human : public Player {
 public:
  Human(const GameScreen *const game, std::string name);
  virtual ~Human();
  virtual bool decide(const Uint8 *const keystate, Bet &b);

 private:
  // if true, user is changing the type. Otherwise, it's the quantity
  bool onType;
};