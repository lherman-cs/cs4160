#include "screens/intro.h"
#include <cmath>
#include <iostream>
#include "global/global.h"
#include "screens/create.h"
#include "screens/game/game.h"
#include "screens/lobby.h"
#include "screens/room.h"
#include "screens/rules.h"
#include "util/ioMod.h"
#include "widget/loading.h"

IntroScreen::IntroScreen() {}
IntroScreen::~IntroScreen() {}

void IntroScreen::onKeyDown(const Uint8* const keystate) {
  // Rules Explanation Screen
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<RulesScreen>();
  }
  // Begin Game
  if (keystate[SDL_SCANCODE_RETURN]) {
    auto loading = Global::get().widget.create<Loading>("Loading...");
    auto& promise = Global::get().promise.add();
    Global::get().mixer.transition.play();
    promise.then(loading->show())
        .sleep(1000)
        .then(loading->dismiss())
        .then([&]() -> bool {
          if (col == 0)
            navigator.push<LobbyScreen>();
          else
            navigator.push<CreateScreen>(difficulty);
          return true;
        });
  }
  // Selection
  if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP] ||
      keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
    row = !row;
  }

  if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
    if (!row) {
      col = !col;
    } else {
      difficulty--;
      if (difficulty < 0) difficulty = 2;
    }
  }

  if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
    if (!row) {
      col = !col;
    } else {
      difficulty = (difficulty + 1) % 3;
    }
  }
}

void IntroScreen::draw() const {
  // World
  introBackground.draw();
  // Current data
  menuWriter.writeText("Join", xstart, ystart,
                       row == 0 && col == 0 ? hoverColor : normalColor);
  menuWriter.writeText("Start", xstart + xstep, ystart,
                       row == 0 && col == 1 ? hoverColor : normalColor);
}

void IntroScreen::update(Uint32 ticks) { (void)ticks; }