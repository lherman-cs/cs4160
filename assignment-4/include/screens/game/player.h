#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include "bet.h"
#include "game.h"

class GameScreen;
class Player {
 public:
  virtual ~Player();
  const std::string name;
  const uint type;
  virtual bool decide(Uint32 ticks, Bet &b) {
    (void)ticks;
    (void)b;
    return false;
  };
  virtual bool decide(const Uint8 *const keystate, Bet &b) {
    (void)keystate;
    (void)b;
    return false;
  };

 protected:
  Player(const GameScreen *const game, const std::string &name, uint type);
  const GameScreen *const game;
};