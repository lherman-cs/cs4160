#pragma once
#include "player/player.h"

class Bot : public Player {
 public:
  Bot(const GameScreen& game, int id);
  virtual ~Bot();
};