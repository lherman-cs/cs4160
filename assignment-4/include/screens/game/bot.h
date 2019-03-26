#pragma once
#include "player.h"

class Bot : public Player {
 public:
  Bot(const GameScreen *const game, int id);
  virtual ~Bot();
  virtual bool decide(Uint32 ticks, int &quantity, int &type);

 private:
  Uint32 elapsed;
};