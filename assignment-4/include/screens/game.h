#pragma once
#include <memory>
#include <vector>
#include "core/event.h"
#include "core/observable.h"
#include "global/navigator.h"
#include "player/player.h"
#include "screen.h"
#include "util/ioMod.h"
#include "util/world.h"

namespace GameEvent {
class CallLiar : public Event {};
}  // namespace GameEvent

class Player;

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
  void validateBet(int& quantity, int& type);

 private:
  struct Bet {
    int quantity;
    int type;
  };
  World background{"screens/game/background"};
  int turn = 0;
  int round = 1;
  std::vector<std::shared_ptr<Player>> players{};
  Bet lastBet{0, 1};
  Bet currBet{1, 1};
  int diceOnTable = 25;

  // UI stuff
  bool onType = false;

  IoMod menuWriter{60};
  IoMod loadingWriter{32};
  Navigator& navigator{Navigator::getInstance()};

  void onDone();
};