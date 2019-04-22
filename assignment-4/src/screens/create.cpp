#include "screens/create.h"
#include "screens/lobby.h"
#include "widget/loading.h"

CreateScreen::CreateScreen(int difficulty) : difficulty(difficulty) {}
CreateScreen::~CreateScreen() {}

void CreateScreen::onKeyDown(const Uint8* const keystate) {
  // Crete room
  if (keystate[SDL_SCANCODE_RETURN]) {
    auto loading = Global::get().widget.create<Loading>("Loading...");
    auto& promise = Global::get().promise.add();
    Global::get().mixer.transition.play();
    promise.then(loading->show())
        .sleep(1000)
        .then(loading->dismiss())
        .then([&]() -> bool {
          navigator.push<LobbyScreen>();
          return true;
        });
  }
  // Delete character
  if (keystate[SDL_SCANCODE_BACKSPACE]) {
    if (name.size() <= 0) return;
    name.pop_back();
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