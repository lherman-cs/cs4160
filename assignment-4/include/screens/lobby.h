#pragma once
#include <vector>
#include "global/global.h"
#include "screens/screen.h"
#include "util/ioMod.h"
#include "util/world.h"

// lobby screen for waiting for networked players to join
//  accepts enter
//  responds to 'enter' start game

class LobbyScreen : public Screen {
 public:
  virtual ~LobbyScreen();
  LobbyScreen(int difficulty, bool isOwner);
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  const World lobbyBackground{"screens/lobby/background"};   // TODO
  const World enterNotification{"screens/lobby/enterable"};  // TODO
  bool isOwner;
  int difficulty;
  int bots;
  std::vector<std::string> names;

  IoMod menuWriter{70};
  Navigator& navigator{Global::get().navigator};

  // Text Positiong
  const int ystart = 30;
  const int ystep = 127;
  const int xstart = 230;
  // Text Rendering
  const SDL_Color normalColor{52, 44, 42, 255};
};