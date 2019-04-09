#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include "bet.h"
#include "game.h"

class GameScreen;
class Bet;
class Player {
 public:
  virtual ~Player();
  const std::string name;
  const uint type;
  virtual bool decide(Uint32 ticks, std::shared_ptr<Bet> bet) {
    (void)ticks;
    (void)bet;
    return false;
  };
  virtual bool decide(const Uint8 *const keystate, std::shared_ptr<Bet> bet) {
    (void)keystate;
    (void)bet;
    return false;
  };

 protected:
  Player(const GameScreen *const game, const std::string &name, uint type);
  const GameScreen *const game;
};