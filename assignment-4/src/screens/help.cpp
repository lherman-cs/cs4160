#include "screens/help.h"
#include <SDL2/SDL_image.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include "util/ioMod.h"

HelpScreen::HelpScreen() : helpBackground("help") {}

HelpScreen::~HelpScreen() {}

void HelpScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    // proceed to Rules
    // engine.currentscreen = "rules"
  }
  if (keystate[SDL_SCANCODE_RETURN]) {
    // proceed to GameScreen
    // engine.currentscreen = "gamescreen"
  }

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

void HelpScreen::draw() const {
  // World
  helpBackground.draw();

  // Current data
  int ystart = 530;
  int ystep = 60;
  int xstart = 630;
  SDL_Color textColor = {88, 53, 31, 255};
  std::stringstream ss;
  ss << (players);
  IoMod::getInstance().writeText(ss.str(), xstart, ystart, textColor);
  ss.str("");
  ss << bots;
  IoMod::getInstance().writeText(ss.str(), xstart, ystart + ystep, textColor);
  ss.str("");
  switch (difficulty) {
    case Difficulty::easy:
      IoMod::getInstance().writeText("E", xstart, ystart + 2 * ystep,
                                     textColor);
      break;
    case Difficulty::medium:
      IoMod::getInstance().writeText("M", xstart, ystart + 2 * ystep,
                                     textColor) break;
    case Difficulty::hard:
      IoMod::getInstance().writeText("H", xstart, ystart + 2 * ystep,
                                     textColor);
      break;
  }
}

void HelpScreen::update(Uint32 ticks) { (void)ticks; }