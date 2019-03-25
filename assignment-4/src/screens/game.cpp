#include "screens/game.h"
#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include "player/bot.h"
#include "player/human.h"
#include "screens/help.h"
#include "util/ioMod.h"

using namespace std::placeholders;

// game screen
//  accepts type and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of type and quantity

GameScreen::GameScreen(int players, int bots, int difficulty)
    : background("game-background"),
      difficulty(difficulty),
      turn(0),
      players(),
      menuWriter(70, {88, 53, 31, 255}),
      navigator(Navigator::getInstance()) {
  auto human = std::make_shared<Human>(this, "Human");
  this->players.emplace_back(human);

  for (int id = 1; id <= bots; id++) {
    auto bot = std::make_shared<Bot>(this, id);
    this->players.emplace_back(bot);
  }
}

GameScreen::~GameScreen() {}

// This is a callback when the player who got the turn finish with
// their turn.
void GameScreen::onDone() {
  turn = (turn + 1) % players.size();
  // TODO! do something useful here
  std::cout << "ON DONE" << std::endl;
}

void GameScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<HelpScreen>();
  }

  int quantity, type;  // probably initialize this with a proper number.
  bool done = players[turn]->decide(keystate, quantity, type);

  // TODO! validate quantity and type here.

  // TODO! update the UI

  if (done) onDone();
}

void GameScreen::draw() const { background.draw(); }

void GameScreen::update(Uint32 ticks) { (void)ticks; }