#pragma once
#include <memory>
#include <vector>
#include "const.h"
#include "core/event.h"
#include "global/global.h"
#include "player.h"
#include "screens/screen.h"
#include "util/ioMod.h"
#include "util/world.h"

class Player;
class Bet;

// game screen
//  accepts type and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of type and quantity
class GameScreen : public Screen {
 public:
  GameScreen(int difficulty, int bots = 4);
  ~GameScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  int getNumDice() const;

 private:
  enum State { Ongoing, CallingLiar, Finish, TurnTransition };
  World background{"screens/game/background"};
  int turn{0};
  int round{1};
  std::vector<std::shared_ptr<Player>> players{};
  int diceOnTable{25};
  std::shared_ptr<Bet> bet;

  IoMod menuWriter{60};
  SDL_Color secondaryColor{182, 148, 103, 255};
  IoMod loadingWriter{32};
  Navigator& navigator{Global::get().navigator};

  void onDone();
  void onCallLiar(std::shared_ptr<Player> caller);
  void removeLosers();
  State state{Ongoing};
};