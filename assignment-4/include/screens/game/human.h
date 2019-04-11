#pragma once
#include <SDL2/SDL.h>
#include "player.h"

class Human : public Player {
 public:
  Human(const GameScreen *const game, Dice d, std::string name);
  virtual ~Human();
  virtual bool decide(const Uint8 *const keystate, std::shared_ptr<Bet> bet);
  virtual bool callLiar(const Uint8 *const keystate);

 private:
  // if true, user is changing the face. Otherwise, it's the quantity
  bool onFace;
};