#include "screens/lobby.h"
#include <algorithm>

LobbyScreen::LobbyScreen() {}

void LobbyScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
    row = std::max((row - 1), 0);
    return;
  }

  if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
    row = std::min((row + 1), static_cast<int>(rooms.size()) - 1);
    return;
  }
}

void LobbyScreen::draw() const {
  background.draw();
  modal.draw();

  auto x = 70;
  auto y = 70;
  auto dy = 80;
  auto count = 0;
  for (const auto& it : rooms) {
    if (count == maxRows) return;
    writer.writeText(it.second, x, y, count == row ? hoverColor : normalColor);
    y += dy;
  }
}

void LobbyScreen::update(Uint32 ticks) {
  if (offline) return;

  if (!subscribed) {
    std::unordered_map<std::string, std::string> req;
    req["command"] = "subscribe";
    try {
      subscribed = session.write(req);
    } catch (...) {
      std::cout << "error occured in connecting. Make it offline" << std::endl;
      offline = true;
    }
  }

  std::unordered_map<std::string, std::string> newRooms;
  try {
    auto done = session.read(newRooms);
    if (done) rooms = newRooms;
  } catch (...) {
    std::cout << "error occured in connecting. Make it offline" << std::endl;
    offline = true;
  }
}