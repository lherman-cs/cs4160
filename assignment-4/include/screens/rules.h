#pragma once
#include "screens/screen.h"
#include "util/world.h"

// rules screen
//  responds to

class rulesScreen : public Screen {
 public:
  virtual ~rulesScreen();
  rulesScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  World rulesBackground;
};