#include "screens/game/game.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include "core/promise.h"
#include "global/clock.h"
#include "screens/game/bot.h"
#include "screens/game/dice.h"
#include "screens/game/human.h"
#include "screens/help.h"
#include "util/ioMod.h"

using namespace std::placeholders;

// game screen
//  accepts face and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of face and quantity

GameScreen::GameScreen(int players, int bots, int difficulty)
    : bet(std::make_shared<Bet>(this)) {
  (void)players;
  (void)difficulty;

#ifdef DEBUG
  int start = 0;
#else
  auto human = std::make_shared<Human>(
      this, Dice(dicePos[0].first, dicePos[0].second), "Human");
  this->players.emplace_back(human);
  int start = 1;
#endif

  for (int id = start; id <= bots; id++) {
    auto bot = std::make_shared<Bot>(
        this, Dice(dicePos[id].first, dicePos[id].second), id);
    this->players.emplace_back(bot);
  }
}

GameScreen::~GameScreen() {}

// Callback for player turn completion.
void GameScreen::onDone() {
  turn = (turn + 1) % players.size();
  round++;

  for (auto player : players) {
    if (player->callLiar(bet)) return onCallLiar(player);
  }
}

int GameScreen::getNumDice() const { return diceOnTable; }

void GameScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<HelpScreen>();
  }

  if (state == State::CallingLiar) return;

  // TODO! check if there's a first bet
  for (auto player : players) {
    if (player->callLiar(keystate, bet)) return onCallLiar(player);
  }

  bool done = players[turn]->decide(keystate, bet);

  if (done) onDone();
}

void GameScreen::update(Uint32 ticks) {
  if (state == State::CallingLiar) return;

  bool done = players[turn]->decide(ticks, bet);
  if (done) onDone();
}

void GameScreen::draw() const {
  background.draw();

  // Draw bet
  bet->draw();

  // Draw round/turn number

  int ystart = 705;
  int xstart = 50;
  int xstep = 110;

  menuWriter.writeText(std::to_string(round), xstart + xstep, ystart,
                       secondaryColor);

  for (const auto player : players) player->draw();

  if (state == State::Ongoing) {
    auto player = players[turn];
    if (player->type == 1) {
      bet->setSelectable(false);
      // If bot, renders a loading text saying that the bot is thinking.
      auto loadingText = player->name + " is thinking...";
      loadingWriter.writeText(loadingText, 680, 720, secondaryColor);
    } else {
      bet->setSelectable(true);
      // Otherwise, notify the human that it is their turn
      loadingWriter.writeText("Your turn", 770, 720, secondaryColor);
    }
  } else if (state == CallingLiar) {
    loadingWriter.writeText("Judging...", 770, 720, secondaryColor);
  }
}

void GameScreen::onCallLiar(std::shared_ptr<Player> caller) {
  state = State::CallingLiar;

  // Show all player's dice
  for (const auto player : players) player->dice.show();
  // wait for some amount of time
  // print out who was correct

  auto& promise = PromiseScheduler::getInstance().add();
  auto judge = [&, caller]() -> bool {
    // Sums all dice
    int sums[6] = {0};
    for (const auto player : players)
      for (const Die die : player->dice.getDice()) sums[die.getValue()]++;

    // Compares to last bet
    if (sums[bet->getLast().face] < bet->getLast().quantity) {
      // The better lied (the caller was right)
      auto playerId = turn - 1;
      if (playerId == -1) playerId = players.size() - 1;
      players[playerId]->dice.remove();
    } else {
      // The better told the truth (the caller was wrong)
      caller->dice.remove();
    }
    return true;
  };
  auto reset = [&]() -> bool {
    for (auto player : players) {
      player->dice.roll();
      if (player->type == 1) player->dice.hide();
    }
    bet->reset();
    diceOnTable--;
    removeLosers();

    state = State::Ongoing;
    return true;
  };

#ifdef DEBUG
  promise.sleep(1).then(judge).sleep(1).then(reset);
#else
  promise.sleep(3).then(judge).sleep(2).then(reset);
#endif
}

void GameScreen::removeLosers() {
  int id = 0;
  auto it = players.begin();
  while (it != players.end()) {
    if ((*it)->dice.getDice().size() == 0) {
      players.erase(it);
      if (id < turn) id--;
      break;
    }
    id++;
    it++;
  }
}