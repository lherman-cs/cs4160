#include "screens/game/game.h"
#include <functional>
#include <iostream>
#include <sstream>
#include "global/clock.h"
#include "screens/game/bot.h"
#include "screens/game/dice.h"
#include "screens/game/human.h"
#include "screens/help.h"
#include "util/ioMod.h"

using namespace std::placeholders;

// game screen
//  accepts type and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of type and quantity

GameScreen::GameScreen(int players, int bots, int difficulty) {
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

// Callback for player turn completion.
void GameScreen::onDone() {
  turn = (turn + 1) % players.size();
  round++;
  // TODO! do something useful here
}

void GameScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<HelpScreen>();
  }

  if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D] ||
      keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT])
    onType ^= 1;

  bool done = players[turn]->decide(keystate, currBet.quantity, currBet.type);

  validateBet(currBet.quantity, currBet.type);

  if (done) onDone();
}

void GameScreen::update(Uint32 ticks) {
  bool done = players[turn]->decide(ticks, currBet.quantity, currBet.type);
  if (done) onDone();
}

void drawTableDice() {
  Dice d = Dice();
  d.roll();
  for (auto p : dice_position) {
    d.positionDie(std::get<0>(p.second), std::get<1>(p.second));
    d.draw();
  }
}

void GameScreen::draw() const {
  SDL_Color normalColor = SDL_Color({52, 44, 42, 255});
  SDL_Color hoverColor = SDL_Color({255, 255, 0, 255});
  SDL_Color secondaryColor = SDL_Color({182, 148, 103, 255});

  background.draw();

  // drawTableDice();

  // Draw bet

  int ystart = 30;
  int xstart = 825;
  int xstep = 110;

  menuWriter.writeText(std::to_string(currBet.quantity), xstart, ystart,
                       !onType ? hoverColor : normalColor);

  Die d(Vector2f(950, 50), Die::State::visible, currBet.type - 1);
  onType ? d.select().draw() : d.deselect().draw();

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
    // Otherwise, notify the human that it is their turn
    loadingWriter.writeText("Your turn", 770, 720, secondaryColor);
}

// validate and update the inputted quantity
void GameScreen::validateBet(int& quantity, int& type) {
  const auto numFaces = 6;  // for dices

  if (quantity < lastBet.quantity)
    quantity = lastBet.quantity;
  else if (quantity > diceOnTable)
    quantity = diceOnTable;
  else if (quantity <= 0)
    quantity = 1;

  if (type < lastBet.type)
    type = lastBet.type;
  else if (type > numFaces)
    type = numFaces;

  if (quantity == lastBet.quantity && type == lastBet.type) {
    if (quantity == diceOnTable)
      type++;
    else
      quantity++;
  }
}
