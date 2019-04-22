#pragma once
#include "global/global.h"
#include "screens/screen.h"
#include "util/ioMod.h"
#include "util/world.h"

// intro screen
//  accepts # of players, # of bots, and difficulty
//  responds to 'h' (rules/help) and 'enter'
//              ← →  for selection

class IntroScreen : public Screen {
 public:
  virtual ~IntroScreen();
  IntroScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  int row = 0;
  int col = 0;
  int difficulty = 0;
  IoMod menuWriter{70};
  Navigator& navigator{Global::get().navigator};

  // Text Positiong
  const int ystart = 500;
  const int ystep = 75;
  const int xstart = 630;
  const int xstep = 200;
  const World introBackground{"screens/intro/background"};
  // Text Rendering
  const SDL_Color normalColor{52, 44, 42, 255};
  const SDL_Color hoverColor{52, 44, 42, 255};
};