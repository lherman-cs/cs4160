#pragma once
#include "global/navigator.h"
#include "screens/screen.h"
#include "util/world.h"

// rules screen
//  responds to 'C' close

class RulesScreen : public Screen {
 public:
  virtual ~RulesScreen();
  RulesScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  const World rulesBackground;
  Navigator& navigator;
};