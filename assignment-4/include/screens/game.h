#pragma once
#include <memory>
#include <vector>
#include "core/event.h"
#include "core/observable.h"
#include "global/navigator.h"
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
  World background;
  int difficulty;
  int turn;
  int players;

  IoMod menuWriter;
  Navigator& navigator;
};