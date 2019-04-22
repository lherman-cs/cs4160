#pragma once
#include "global/global.h"
#include "screens/screen.h"
#include "util/ioMod.h"
#include "util/world.h"

// create screen
//  accepts # of players, # of bots, and difficulty
//  responds to name inputting and 'enter' (start game)

class CreateScreen : public Screen {
 public:
  virtual ~CreateScreen();
  CreateScreen(int difficulty);
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  const World createBackground{"screens/create/background"};  // todo
  const uint maxChar{10};
  std::string name;
  int difficulty;

  // 4-29 is A-Z for SDL input (no case)
  const uint SDL_A = 4;
  const uint SDL_Z = 29;
  // 65-90 is ascii A-Z (uppercase)
  const uint ASCII_A = 65;

  IoMod menuWriter{70};
  Navigator& navigator{Global::get().navigator};

  // Text Positiong
  const int ystart = 300;
  const int xstart = 500;
  // Text Rendering
  const SDL_Color normalColor{52, 44, 42, 255};
};