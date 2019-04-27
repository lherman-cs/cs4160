#include "screens/create.h"
#include "net/tcp.h"
#include "screens/room.h"
#include "widget/loading.h"

CreateScreen::CreateScreen(int difficulty) : difficulty(difficulty) {}
CreateScreen::~CreateScreen() {}

void CreateScreen::onKeyDown(const Uint8* const keystate) {
  // Crete room
  if (keystate[SDL_SCANCODE_RETURN]) {
    auto loading = Global::get().widget.create<Loading>("Loading...");
    auto& promise = Global::get().promise.add();
    auto gameSession = std::make_shared<TCP>();
    auto req = net::create(name);
    auto resp = std::make_shared<net::message>();

    auto requesting = [=]() {
      // if write fails, just return, dont keep looping. Probably give some
      //   feedback to the user
      if (gameSession->isOffline()) return true;
      auto done = gameSession->write(*req);
      return done;
    };

    auto confirming = [=]() {
      // if read fails, just return, dont keep looping.
      // Probably give some user feedbacks.
      if (gameSession->isOffline()) return true;
      auto done = gameSession->read(*resp);
      if (!done) return false;

      if (resp->find("error") != resp->end()) {
        return true;
      }

      navigator.push<RoomScreen>(gameSession, difficulty, true);
      return true;
    };

    Global::get().mixer.transition.play();
    promise.then(loading->show())
        .then(requesting)
        .then(confirming)
        .then(loading->dismiss());
    return;
  }

  // Delete character
  if (keystate[SDL_SCANCODE_BACKSPACE]) {
    if (name.size() <= 1) return;
    name.pop_back();
    return;
  }

  if (keystate[SDL_SCANCODE_SPACE]) {
    if (name.size() >= maxChar) return;
    name.push_back(' ');
    return;
  }

  // Add character
  for (uint ch = SDL_A; ch <= SDL_Z; ch++) {
    if (keystate[ch]) {
      if (name.size() >= maxChar) return;
      name.push_back(ch + (ASCII_A - SDL_A));
    }
  }
}

void CreateScreen::draw() const {
  createBackground.draw();
  menuWriter.writeText(name, xstart, ystart, normalColor);
}

void CreateScreen::update(Uint32 ticks) { (void)ticks; }