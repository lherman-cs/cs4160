#include "screens/rules.h"

RulesScreen::RulesScreen() {}
RulesScreen::~RulesScreen() {}

void RulesScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_C]) {
    navigator.pop();
  }
}

void RulesScreen::draw() const { rulesBackground.draw(); }

void RulesScreen::update(Uint32 ticks) { (void)ticks; }