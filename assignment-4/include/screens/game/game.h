#pragma once
#include <memory>
#include <vector>
#include "const.h"
#include "core/event.h"
#include "global/global.h"
#include "net/state.h"
#include "player.h"
#include "screens/screen.h"
#include "util/ioMod.h"
#include "util/world.h"

class State;

// game screen
//  accepts type and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of type and quantity
class GameScreen : public Screen {
 public:
  GameScreen(int difficulty);
  ~GameScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  int getNumDice() const;

 private:
  enum Status { Ongoing, CallingLiar, Finish, TurnTransition };

  Status status{Ongoing};
  State state{};

  IoMod menuWriter{60};
  SDL_Color secondaryColor{182, 148, 103, 255};
  IoMod loadingWriter{32};
  World background{"screens/game/background"};

  Navigator& navigator{Global::get().navigator};

  void onDone();
  void onCallLiar(std::shared_ptr<Player> caller);
  void removeLosers();
};