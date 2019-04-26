#pragma once
#include <vector>
#include "global/global.h"
#include "net/tcp.h"
#include "screens/screen.h"
#include "util/ioMod.h"
#include "util/world.h"

// room screen for waiting for networked players to join
//  accepts enter
//  responds to 'enter' start game

class RoomScreen : public Screen {
 public:
  virtual ~RoomScreen();
  RoomScreen(std::shared_ptr<TCP> session, int difficulty, bool isOwner);
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  const World lobbyBackground{"screens/lobby/background"};   // TODO
  const World enterNotification{"screens/lobby/enterable"};  // TODO
  std::shared_ptr<TCP> session;
  const std::string id;
  bool isOwner;
  int difficulty;
  int bots;
  std::vector<std::string> names;
  bool connected = false;

  IoMod menuWriter{70};
  Navigator& navigator{Global::get().navigator};

  // Text Positiong
  const int ystart = 30;
  const int ystep = 127;
  const int xstart = 230;
  // Text Rendering
  const SDL_Color normalColor{52, 44, 42, 255};
};