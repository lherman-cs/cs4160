#include "screens/game.h"
#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include "global/clock.h"
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
      lastBet({0, 1}),
      currBet({1, 1}),
      menuWriter(60),
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

  bool done = players[turn]->decide(keystate, currBet.quantity, currBet.type);

  validateBet(currBet.quantity, currBet.type);

  if (done) onDone();
}

void GameScreen::update(Uint32 ticks) {
  bool done = players[turn]->decide(ticks, currBet.quantity, currBet.type);
  if (done) onDone();
}

void GameScreen::draw() const {
  background.draw();

  // Current data
  int ystart = 30;
  int xstart = 825;
  int xstep = 110;
  std::stringstream ss;
  ss << currBet.quantity;
  menuWriter.writeText(ss.str(), xstart, ystart, {52, 44, 42, 255});
  ss.str("");
  ss << currBet.type;
  menuWriter.writeText(ss.str(), xstart + xstep, ystart, {52, 44, 42, 255});
  ss.str("");
  ystart = 705;
  xstart = 50;
  ss << turn;
  menuWriter.writeText(ss.str(), xstart + xstep, ystart, {182, 148, 103, 255});
  ss.str("");
}

// validate and update the inputted quantity
void GameScreen::validateBet(int& quantity, int& type) {
  quantity = quantity > diceOnTable ? diceOnTable : quantity;
  type = type > 6 ? 6 : type;

  if (quantity >= lastBet.quantity && type >= lastBet.type &&
      (quantity > lastBet.quantity || type > lastBet.type))
    return;

  /**
   * Case 1:
   * - quantity or type is less than the last bet.
   *
   * Case 2:
   * - quantity and type are equal to the last bet.
   *
   * Case 3:
   * - quantity or type is greater than 25 or 6 respectively.
   */

  auto incType = [&]() {
    if (type < 6) {
      type++;
      return;
    }
    // if (quantity <= lastBet.quantity && lastBet.type == 6) return incQuan();
    type = lastBet.type;
    type += quantity > lastBet.quantity ? 0 : 1;
  };

  auto incQuan = [&]() {
    if (quantity < diceOnTable) {
      quantity++;
      return;
    }
    if (lastBet.quantity == diceOnTable) return incType();
    quantity = lastBet.quantity;
    quantity += type > lastBet.type ? 0 : 1;
  };

  if (type < lastBet.type) {
    type = lastBet.type;
    return;
  }
  incQuan();
}
