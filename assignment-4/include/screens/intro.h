#pragma once
#include "global/global.h"
#include "screens/screen.h"
#include "util/ioMod.h"
#include "util/world.h"

// intro screen
//  accepts # of players, # of bots, and difficulty
//  responds to 'h' rules and 'enter' start game
//              ← → ↑ ↓ for selection of players and bots

class IntroScreen : public Screen {
 public:
  virtual ~IntroScreen();
  IntroScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  const World introBackground{"screens/intro/background"};
  int row = 0;
  int players = 5;
  int bots = 4;
  int difficulty = 0;

  IoMod menuWriter{70};
  Navigator& navigator{Global::get().navigator};
};