#pragma once
#include "global/global.h"
#include "screens/screen.h"
#include "util/world.h"

// playerInfo screen
//  responds to 'C' close

class PlayerInfoScreen : public Screen {
 public:
  PlayerInfoScreen();
  virtual ~PlayerInfoScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  const World playerInfoBackground{"screens/player-info/background"};
  Navigator& navigator{Global::get().navigator};
};