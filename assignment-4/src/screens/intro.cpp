#include "screens/intro.h"
#include <cmath>
#include <iostream>
#include "core/promise.h"
#include "screens/game/game.h"
#include "screens/rules.h"
#include "util/ioMod.h"
#include "widget/loading.h"

IntroScreen::IntroScreen() {}
IntroScreen::~IntroScreen() {}

void IntroScreen::onKeyDown(const Uint8* const keystate) {
  // Rules Explination Screen
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<RulesScreen>();
  }
  // Begin Game
  if (keystate[SDL_SCANCODE_RETURN]) {
    auto& loading = Loading::getInstance();
    auto& promise = PromiseScheduler::getInstance().add();
    promise.then(loading.show)
        .sleep(1)
        .then(loading.dismiss)
        .then([&]() -> bool {
          navigator.push<GameScreen>(players, bots, difficulty);
          return true;
        });
  }
  // Selection
  if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
    row--;
    if (row < 0) row = 2;
  }
  if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
    row = (row + 1) % 3;
  }

  if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
    switch (row) {
      case 2:
        difficulty--;
        if (difficulty < 0) difficulty = 2;
        break;
    }
  }

  if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
    switch (row) {
      case 2:
        difficulty = (difficulty + 1) % 3;
        break;
    }
  }
}

void IntroScreen::draw() const {
  auto normalColor = SDL_Color({52, 44, 42, 255});
  auto hoverColor = SDL_Color({255, 255, 0, 255});

  // World
  introBackground.draw();

  // Current data
  int ystart = 500;
  int ystep = 75;
  int xstart = 630;

  menuWriter.writeText(std::to_string(players), xstart, ystart,
                       row == 0 ? hoverColor : normalColor);
  menuWriter.writeText(std::to_string(bots), xstart, ystart + ystep,
                       row == 1 ? hoverColor : normalColor);
  auto color = row == 2 ? hoverColor : normalColor;
  switch (difficulty) {
    case 0:
      menuWriter.writeText("E", xstart, ystart + 2 * ystep, color);
      break;
    case 1:
      menuWriter.writeText("M", xstart, ystart + 2 * ystep, color);
      break;
    case 2:
      menuWriter.writeText("H", xstart, ystart + 2 * ystep, color);
      break;
  }
}

void IntroScreen::update(Uint32 ticks) { (void)ticks; }