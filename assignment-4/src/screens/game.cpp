#include "screens/game.h"
#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include "player/bot.h"
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
      turn(1),
      players(players),
      lastBet({0, 1}),
      menuWriter(70, {88, 53, 31, 255}),
      navigator(Navigator::getInstance()) {}

GameScreen::~GameScreen() {}

void GameScreen::onKeyDown(const Uint8* const keystate) {}

void GameScreen::draw() const { background.draw(); }

void GameScreen::update(Uint32 ticks) { (void)ticks; }

// validate and update the inputted quantity
void GameScreen::validateBet(int& quantity, int& type) {
  if (quantity > lastBet.quantity || type > lastBet.type) return;

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
