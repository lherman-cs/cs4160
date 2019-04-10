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

GameScreen::GameScreen(int players, int bots, int difficulty)
    : bet(std::make_shared<Bet>(this)) {
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

int GameScreen::getNumDice() const { return diceOnTable; }
bool GameScreen::getSelected() const { return onType; }

void GameScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<HelpScreen>();
  }

  // if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D] ||
  //     keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT]) {
  //   onType ^= 1;
  //   bet->selectDie(onType);
  // }

  bool done = players[turn]->decide(keystate, bet);

  if (done) onDone();
}

void GameScreen::update(Uint32 ticks) {
  bool done = players[turn]->decide(ticks, bet);
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
  SDL_Color secondaryColor = SDL_Color({182, 148, 103, 255});

  background.draw();

  // drawTableDice();

  // Draw bet
  bet->draw();

  // Draw round/turn number

  int ystart = 705;
  int xstart = 50;
  int xstep = 110;

  menuWriter.writeText(std::to_string(round), xstart + xstep, ystart,
                       secondaryColor);

  auto player = players[turn];
  if (player->type == 1) {
    // bet->setSelectable(false);
    // If bot, renders a loading text saying that the bot is thinking.
    auto loadingText = player->name + " is thinking...";
    loadingWriter.writeText(loadingText, 680, 720, secondaryColor);
  } else {
    // bet->setSelectable(true);
    // Otherwise, notify the human that it is their turn
    loadingWriter.writeText("Your turn", 770, 720, secondaryColor);
  }
}