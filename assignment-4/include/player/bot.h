#pragma once
#include "player/player.h"

class Bot : public Player {
 public:
  Bot(const GameScreen* const game, int id);
  virtual ~Bot();
};