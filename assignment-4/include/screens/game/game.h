#pragma once
#include <memory>
#include <vector>
#include "const.h"
#include "core/event.h"
#include "core/observable.h"
#include "global/navigator.h"
#include "player.h"
#include "screens/screen.h"
#include "util/ioMod.h"
#include "util/world.h"

namespace GameEvent {
class CallLiar : public Event {};
}  // namespace GameEvent

class Player;
class Bet;

// game screen
//  accepts type and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of type and quantity
class GameScreen : public Screen {
 public:
  GameScreen(int players, int bots, int difficulty);
  ~GameScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  int getNumDice() const;

 private:
  World background{"screens/game/background"};
  int turn = 0;
  int round = 1;
  std::vector<std::shared_ptr<Player>> players{};
  int diceOnTable = 25;
  std::shared_ptr<Bet> bet;

  IoMod menuWriter{60};
  SDL_Color secondaryColor{182, 148, 103, 255};
  IoMod loadingWriter{32};
  Navigator& navigator{Navigator::getInstance()};

  void onDone();
};