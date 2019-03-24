#pragma once
#include <memory>
#include <vector>
#include "core/event.h"
#include "core/observable.h"
#include "global/navigator.h"
// #include "player/player.h"
#include "screen.h"
#include "util/ioMod.h"
#include "util/world.h"

namespace GameEvent {
class CallLiar : public Event {};
class SetBet : public Event {
 public:
  SetBet(uint64_t quantity, uint64_t type) : quantity(quantity), type(type) {}
  const uint64_t quantity, type;
};
}  // namespace GameEvent

// game screen
//  accepts type and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of type and quantity

class GameScreen : public Screen {
 public:
  GameScreen(int players, int bots, int difficulty);
  virtual ~GameScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  enum Column { type, quantity };
  struct Bet {
    short quantity;
    short type;
  };

  World background;
  int difficulty;
  int turn;
  int column;
  int players;
  Bet lastBet;
  Bet currBet;
  int diceOnTable = 25;

  // std::vector<std::unique_ptr<Player>> players;

  IoMod menuWriter;
  Navigator& navigator;

  void onCallLiar(const GameEvent::CallLiar& e);
  void onSetBet(const GameEvent::SetBet& e);

  void incType();
  void incQuan();
  void decType();
  void decQuan();
};