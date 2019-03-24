#pragma once
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
  enum Difficulty { easy, medium, hard };
  enum Row { player, bot, difficult };
  World introBackground;
  int row;
  int players;
  int bots;
  int difficulty;

  IoMod menuWriter;
};