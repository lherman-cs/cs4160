#pragma once
#include <unordered_map>
#include "net/tcp.h"
#include "screen.h"
#include "util/world.h"

class LobbyScreen : public Screen {
 public:
  LobbyScreen();
  virtual void onKeyDown(const Uint8* const keystate);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

 private:
  // Networking
  TCP session{};
  IoMod writer{50};
  bool subscribed = false;
  bool joining = false;
  std::unordered_map<std::string, std::string> rooms{};

  // Listing Rooms
  int row = 0;
  int maxRows = 5;

  // Draw
  const World background{"screens/intro/background"};  // TODO
  const World modal{"screens/lobby/modal"};
  const SDL_Color normalColor{52, 44, 42, 255};
  const SDL_Color hoverColor{255, 255, 0, 255};
};