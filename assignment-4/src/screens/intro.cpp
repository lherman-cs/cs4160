#include "screens/intro.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include "screens/game.h"
#include "screens/rules.h"
#include "util/ioMod.h"

IntroScreen::IntroScreen()
    : introBackground("intro"),
      row(0),
      players(5),
      bots(4),
      difficulty(0),
      menuWriter(70, {88, 53, 31, 255}),
      navigator(Navigator::getInstance()) {}

IntroScreen::~IntroScreen() {}

void IntroScreen::onKeyDown(const Uint8* const keystate) {
  // Rules Explination Screen
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<RulesScreen>();
  }
  // Begin Game
  if (keystate[SDL_SCANCODE_RETURN]) {
    navigator.push<GameScreen>(players, bots, difficulty);
  }
  // Selection
  if (keystate[SDL_SCANCODE_W]) {
    row--;
    if (row < 0) row = 2;
  }
  if (keystate[SDL_SCANCODE_S]) {
    row = (row + 1) % 3;
  }

  if (keystate[SDL_SCANCODE_A]) {
    switch (row) {
      case Row::difficult:
        difficulty--;
        if (difficulty < 0) difficulty = 2;
        break;
    }
  }

  if (keystate[SDL_SCANCODE_D]) {
    switch (row) {
      case Row::difficult:
        difficulty = (difficulty + 1) % 3;
        break;
    }
  }
}

void IntroScreen::draw() const {
  // World
  introBackground.draw();

  // Current data
  int ystart = 500;
  int ystep = 75;
  int xstart = 630;
  std::stringstream ss;
  ss << (players);
  menuWriter.writeText(ss.str(), xstart, ystart);
  ss.str("");
  ss << bots;
  menuWriter.writeText(ss.str(), xstart, ystart + ystep);
  ss.str("");
  switch (difficulty) {
    case 0:
      menuWriter.writeText("E", xstart, ystart + 2 * ystep);
      break;
    case 1:
      menuWriter.writeText("M", xstart, ystart + 2 * ystep);
      break;
    case 2:
      menuWriter.writeText("H", xstart, ystart + 2 * ystep);
      break;
  }
}

void IntroScreen::update(Uint32 ticks) { (void)ticks; }