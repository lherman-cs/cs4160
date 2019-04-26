#include "screens/room.h"
#include "net/tcp.h"
#include "screens/game/game.h"
#include "widget/loading.h"

RoomScreen::RoomScreen(std::shared_ptr<TCP> session, int difficulty,
                       bool isOwner)
    : session(session), id(id), difficulty(difficulty), isOwner(isOwner) {
  names.emplace_back(isOwner ? "You" : "You (Not Owner)");
  for (int i = 1; i < 5; i++) {
    names.emplace_back("Bot " + std::to_string(i));
  }
}
RoomScreen::~RoomScreen() {}

void RoomScreen::onKeyDown(const Uint8* const keystate) {
  // Begin Game (only available if you are the room owner)
  if (isOwner && keystate[SDL_SCANCODE_RETURN]) {
    auto loading = Global::get().widget.create<Loading>("Loading...");
    auto& promise = Global::get().promise.add();
    Global::get().mixer.transition.play();
    promise.then(loading->show())
        .sleep(1000)
        .then(loading->dismiss())
        .then([&]() -> bool {
          navigator.push<GameScreen>(difficulty);
          return true;
        });
  }
}

void RoomScreen::draw() const {
  lobbyBackground.draw();
  // if owner, display enter (an asset)
  if (isOwner) enterNotification.draw();
  // player list
  for (int i = 0; i < names.size(); i++) {
    menuWriter.writeText(names[i], xstart, ystart + i * ystep, normalColor);
  }
}

void RoomScreen::update(Uint32 ticks) { (void)ticks; }