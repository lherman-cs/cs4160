#pragma once
#include "player.h"

class Bot : public Player {
 public:
  Bot(const GameScreen* const game, Dice d, int id);
  virtual ~Bot();
  virtual bool decide(Uint32 ticks, std::shared_ptr<Bet> bet);
  virtual bool callLiar(const std::shared_ptr<const Bet> bet);

 private:
  Uint32 elapsed;
};