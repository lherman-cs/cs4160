#pragma once
#include "global/global.h"
#include "screens/screen.h"
#include "util/ioMod.h"
#include "util/world.h"

// create screen
//  responds to name inputting and 'enter' (start game)

class CreateScreen : public Screen {
 public:
  virtual ~CreateScreen();
  CreateScreen(int difficulty);
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  // State Management
  bool entering{false};

  // Game Data
  int difficulty;

  // Room Name Input
  std::string name{" "};
  const uint maxChar{11};
  // 4-29 is A-Z for SDL input (no case)
  const uint SDL_A = 4;
  const uint SDL_Z = 29;
  // 65-90 is ascii A-Z (uppercase)
  const uint ASCII_A = 65;

  // Drawing
  IoMod menuWriter{60};
  Navigator& navigator{Global::get().navigator};
  const World createBackground{"screens/create/background"};  // todo

  // Text Positiong
  const int ystart = 250;
  const int xstart = 300;

  // Text Rendering
  const SDL_Color normalColor{52, 44, 42, 255};
};