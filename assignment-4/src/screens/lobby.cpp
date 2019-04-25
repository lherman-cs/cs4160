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
  if (offline) return;
  modal.draw();

  auto x = 150;
  auto y = 150;
  auto dy = 80;
  auto count = 0;
  for (const auto& it : rooms) {
    if (count == maxRows) return;
    writer.writeText(it.second, x, y, count == row ? hoverColor : normalColor);
    y += dy;
    count++;
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
  } else {
    std::unordered_map<std::string, std::string> newRooms;
    try {
      auto done = session.read(newRooms);
      if (done) rooms = newRooms;
    } catch (...) {
      std::cout << "error occured in connecting. Make it offline" << std::endl;
      offline = true;
    }
  }

  if (offline) {
    auto loading = Global::get().widget.create<Loading>("You're offline");
    auto redirecting = Global::get().widget.create<Loading>("Going back");
    auto goBack = []() {
      Global::get().navigator.pop();
      return true;
    };

    auto& promise = Global::get().promise.add();
    promise.then(loading->show())
        .sleep(2000)
        .then(loading->dismiss())
        .then(redirecting->show())
        .sleep(500)
        .then(goBack)
        .sleep(500)
        .then(redirecting->dismiss());
  }
}