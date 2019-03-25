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

GameScreen::GameScreen(int players, int bots, int difficulty) {
  // TODO! do something with this variable.
  (void)players;
  (void)difficulty;

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
  round++;
  // TODO! do something useful here
  std::cout << "ON DONE" << std::endl;
}

void GameScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<HelpScreen>();
  }

  if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D]) onType ^= 1;

  bool done = players[turn]->decide(keystate, currBet.quantity, currBet.type);

  validateBet(currBet.quantity, currBet.type);

  if (done) onDone();
}

void GameScreen::update(Uint32 ticks) {
  bool done = players[turn]->decide(ticks, currBet.quantity, currBet.type);
  if (done) onDone();
}

void GameScreen::draw() const {
  auto normalColor = SDL_Color({52, 44, 42, 255});
  auto hoverColor = SDL_Color({255, 255, 0, 255});
  auto secondaryColor = SDL_Color({182, 148, 103, 255});

  background.draw();

  // Current data
  int ystart = 30;
  int xstart = 825;
  int xstep = 110;
  menuWriter.writeText(std::to_string(currBet.quantity), xstart, ystart,
                       !onType ? hoverColor : normalColor);
  menuWriter.writeText(std::to_string(currBet.type), xstart + xstep, ystart,
                       onType ? hoverColor : normalColor);

  ystart = 705;
  xstart = 50;
  menuWriter.writeText(std::to_string(round), xstart + xstep, ystart,
                       secondaryColor);

  // If bot, renders a loading text saying that the bot is thinking.
  auto player = players[turn];
  if (player->type == 1) {
    auto loadingText = player->name + " is thinking...";
    loadingWriter.writeText(loadingText, 680, 720, secondaryColor);
  } else
    loadingWriter.writeText("Your turn", 770, 720, secondaryColor);
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
