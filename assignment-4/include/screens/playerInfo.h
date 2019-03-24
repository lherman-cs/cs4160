#pragma once
#include "screens/screen.h"
#include "util/world.h"

// playerInfo screen
//  responds to

class playerInfoScreen : public Screen {
 public:
  virtual ~playerInfoScreen();
  playerInfoScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  World playerInfoBackground;
};