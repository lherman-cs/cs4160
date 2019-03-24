#include "screens/playerInfo.h"

PlayerInfoScreen::PlayerInfoScreen()
    : playerInfoBackground("playerInfo"), navigator(Navigator::getInstance()) {}

PlayerInfoScreen::~PlayerInfoScreen() {}

void PlayerInfoScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_C]) {
    navigator.pop();
  }
}

void PlayerInfoScreen::draw() const { playerInfoBackground.draw(); }

void PlayerInfoScreen::update(Uint32 ticks) { (void)ticks; }