#pragma once
#include <memory>
#include <vector>
#include "core/event.h"
#include "core/observable.h"
#include "global/navigator.h"
#include "player.h"
#include "screens/game/include.h"
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
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
class GameScreen : public Screen,
                   public std::enable_shared_from_this<GameScreen> {
 public:
  GameScreen(int players, int bots, int difficulty);
  ~GameScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  int getNumDice() const;
  bool getSelected() const;

 private:
  World background{"screens/game/background"};
  int turn = 0;
  int round = 1;
  std::vector<std::shared_ptr<Player>> players{};
  int diceOnTable = 25;
  std::shared_ptr<Bet> bet;

  // UI stuff
  bool onType = false;

  IoMod menuWriter{60};
  IoMod loadingWriter{32};
  Navigator& navigator{Navigator::getInstance()};

  void onDone();
};