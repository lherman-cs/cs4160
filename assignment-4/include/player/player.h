#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "screens/game.h"

class GameScreen;
class Player {
 public:
  virtual ~Player();
  const std::string name;
  const uint type;
  virtual bool decide(Uint32 ticks, int &quantity, int &type) {
    (void)ticks;
    (void)quantity;
    (void)type;
    return false;
  };
  virtual bool decide(const Uint8 *const keystate, int &quantity, int &type) {
    (void)keystate;
    (void)quantity;
    (void)type;
    return false;
  };

 protected:
  Player(const GameScreen *const game, const std::string &name, uint type);
  const GameScreen *const game;
};