#include "screens/room.h"
#include <sstream>
#include <string>
#include "net/game.h"
#include "net/tcp.h"
#include "widget/loading.h"

RoomScreen::RoomScreen(std::shared_ptr<TCP> session, int difficulty,
                       bool isOwner, int index)
    : session(session),
      isOwner(isOwner),
      names(),
      difficulty(difficulty),
      index(index) {}

RoomScreen::~RoomScreen() {}

void RoomScreen::onKeyDown(const Uint8* const keystate) {
  // Begin Game (only available if you are the room owner)
  if (isOwner && keystate[SDL_SCANCODE_RETURN]) {
    auto loading = Global::get().widget.create<Loading>("Loading...");
    auto& promise = Global::get().promise.add();
    auto msg = net::gameStart();
    promise.then(loading->show())
        .then([=]() { return session->write(*msg); })
        .then(loading->dismiss());
  }
}

void RoomScreen::draw() const {
  lobbyBackground.draw();
  // if owner, display enter (an asset)
  if (isOwner) enterNotification.draw();
  // player list
  for (unsigned int i = 0; i < names.size(); i++) {
    auto name = names[i].substr(0, 8);
    if (i == static_cast<unsigned int>(index)) name += "(you)";
    menuWriter.writeText(name, xstart, ystart + i * ystep, normalColor);
  }
}

void RoomScreen::update(Uint32 ticks) {
  (void)ticks;

  // do nothing when it's offline
  if (session->isOffline()) return;
  if (starting) return;

  net::message msg;
  bool ready = session->read(msg);
  if (!ready) return;

  auto type = msg["type"];
  if (type == "state") {
    auto players = msg["players"];
    std::replace(players.begin(), players.end(), ',', ' ');
    names.clear();

    std::stringstream iss(players);
    std::string player;
    while (iss >> player) names.emplace_back(player);

  } else if (type == "start") {
    starting = true;
    auto loading = Global::get().widget.create<Loading>("Loading...");
    auto& promise = Global::get().promise.add();
    Global::get().mixer.transition.play();
    promise.then(loading->show())
        .sleep(1000)
        .then(loading->dismiss())
        .then([=]() {
          auto& navigator = Global::get().navigator;
          navigator.push<NetGameScreen>(session, index, difficulty);
          return true;
        });
  }
}