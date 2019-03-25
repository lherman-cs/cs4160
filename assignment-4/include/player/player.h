#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "core/observable.h"
#include "screens/game.h"

class GameScreen;
class Player : public Observable {
 public:
  virtual ~Player();
  const std::string name;
  const uint type;
  virtual void decide(Uint32 ticks, int &quantity, int &type) {
    (void)ticks;
    (void)quantity;
    (void)type;
  };
  virtual void decide(const Uint8 *const keystate, int &quantity, int &type) {
    (void)keystate;
    (void)quantity;
    (void)type;
  };

 protected:
  Player(const GameScreen *const game, const std::string &name, uint type);
  const GameScreen *const game;
};