#include "screens/lobby.h"
#include <assert.h>
#include <algorithm>
#include "screens/room.h"

LobbyScreen::LobbyScreen() {}

void LobbyScreen::onKeyDown(const Uint8* const keystate) {
  if (joining) return;

  if (keystate[SDL_SCANCODE_Q]) {
    Global::get().navigator.pop();
    return;
  }

  if (keystate[SDL_SCANCODE_RETURN]) {
    auto it = rooms.begin();
    auto count = 0;
    while (count != row) {
      if (it == rooms.end()) return;
      it++;
    }

    auto id = it->first;
    auto& promise = Global::get().promise.add();
    auto gameSession = std::make_shared<TCP>();
    auto req = net::join(id);
    auto resp = std::make_shared<net::message>();
    joining = true;

    auto requesting = [=]() {
      // if write fails, just return, dont keep looping. Probably give some
      //   feedback to the user
      if (gameSession->isOffline()) return true;
      bool done = gameSession->write(*req);
      return done;
    };

    auto confirming = [=]() {
      // if read fails, just return, dont keep looping. Probably give some
      //   feedback to the user
      if (gameSession->isOffline()) {
        joining = false;
        return true;
      }
      bool done = gameSession->read(*resp);
      if (!done) return false;

      if (resp->find("error") != resp->end()) {
        joining = false;
        return true;
      }
      assert(resp->find("type") != resp->end());
      std::string type = (*(resp->find("type"))).second;
      assert(type == "join");
      std::string indexString = (*(resp->find("index"))).second;
      int index = std::stoi(indexString);

      auto& navigator = Global::get().navigator;
      navigator.pop();
      navigator.push<RoomScreen>(gameSession, 0, false, index);
      return true;
    };

    promise.then(requesting).then(confirming);
    return;
  }

  if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
    row = std::max((row - 1), 0);
    return;
  }

  if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
    row = std::min((row + 1), std::min(maxRows - 1, (int)rooms.size() - 1));
    return;
  }
}

void LobbyScreen::draw() const {
  background.draw();
  if (session.isOffline()) return;
  modal.draw();

  auto x = 200;
  auto y = 150;
  auto dy = 80;
  auto count = 0;
  for (const auto& it : rooms) {
    if (count == maxRows) return;
    auto info = it.second;
    std::replace(info.begin(), info.end(), ',', ' ');
    std::stringstream iss(info);
    std::string name;
    std::string numPlayers;
    iss >> name >> numPlayers;
    auto text = name + " (" + numPlayers + "/5)";

    writer.writeText(text, x, y, count == row ? hoverColor : normalColor);
    y += dy;
    count++;
  }
}

void LobbyScreen::update(Uint32 ticks) {
  if (session.isOffline()) return;
  if (joining) return;

  if (!subscribed) {
    std::unordered_map<std::string, std::string> req;
    req["command"] = "subscribe";
    subscribed = session.write(req);
  } else {
    std::unordered_map<std::string, std::string> newRooms;
    auto done = session.read(newRooms);
    if (done) rooms = newRooms;
  }

  if (session.isOffline()) {
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