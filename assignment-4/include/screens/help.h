#pragma once
#include "screens/screen.h"
#include "util/world.h"

// help screen
//  responds to '

class HelpScreen : public Screen {
 public:
  virtual ~HelpScreen();
  HelpScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  World helpBackground;
};