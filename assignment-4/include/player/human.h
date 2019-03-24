#pragma once
#include <SDL2/SDL.h>
#include "player/player.h"

class Human : public Player {
 public:
  Human(const GameScreen& game, std::string name);
  virtual ~Human();
  void onKeyDown(const Uint8* const keystate);
};