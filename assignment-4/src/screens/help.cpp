#include "screens/help.h"
#include "screens/playerInfo.h"
#include "screens/rules.h"

HelpScreen::HelpScreen()
    : helpBackground("help"), navigator(Navigator::getInstance()) {}

HelpScreen::~HelpScreen() {}

void HelpScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<RulesScreen>();
  }
  if (keystate[SDL_SCANCODE_P]) {
    navigator.push<PlayerInfoScreen>();
  }
  if (keystate[SDL_SCANCODE_C]) {
    navigator.pop();
  }
}

void HelpScreen::draw() const { helpBackground.draw(); }

void HelpScreen::update(Uint32 ticks) { (void)ticks; }