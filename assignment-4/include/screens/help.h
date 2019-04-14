#pragma once
#include "global/global.h"
#include "screens/screen.h"
#include "util/world.h"

// help screen
//  responds to 'H' rules, 'P' player info, 'C' close menu

class HelpScreen : public Screen {
 public:
  virtual ~HelpScreen();
  HelpScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  const World helpBackground{"screens/help/background"};
  Navigator& navigator{Global::get().navigator};
};