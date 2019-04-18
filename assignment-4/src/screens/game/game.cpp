#include "screens/game/game.h"
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include "global/global.h"
#include "screens/game/bot.h"
#include "screens/game/dice.h"
#include "screens/game/human.h"
#include "screens/help.h"
#include "util/ioMod.h"
#include "widget/loading.h"

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

  // Allows each bot to get an equal chance of calling liar first, prevents
  //   first bot from always getting to go first
  std::vector<std::shared_ptr<Player>> temp = players;
  std::shuffle(
      std::begin(temp), std::end(temp),
      std::default_random_engine(
          std::chrono::system_clock::now().time_since_epoch().count()));
  for (auto player : temp) {
    if (player->callLiar(bet)) return onCallLiar(player);
  }

  state = State::TurnTransition;
  auto& promise = Global::get().promise.add();
  auto loading =
      Global::get().widget.create<Loading>(players[turn]->name + "'s turn");

  promise.then(loading->show())
      .sleep(1300)
      .then(loading->dismiss())
      .then([&]() -> bool {
        state = State::Ongoing;
        return true;
      });
}

int GameScreen::getNumDice() const { return diceOnTable; }

void GameScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<HelpScreen>();
  }

  if (state == State::CallingLiar || state == State::Finish) return;

  // TODO! check if there's a first bet

  for (auto player : players) {
    if (player->callLiar(keystate, bet)) return onCallLiar(player);
  }

  bool done = players[turn]->decide(keystate, bet);

  if (done) onDone();
}

void GameScreen::update(Uint32 ticks) {
  if (state == State::CallingLiar || state == State::Finish ||
      state == State::TurnTransition)
    return;

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
  if (state == State::TurnTransition) return;

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
  } else {
    const auto finishText = players[0]->name + " won!";
    loadingWriter.writeText(finishText, 750, 720, secondaryColor);
  }
}

void GameScreen::onCallLiar(std::shared_ptr<Player> caller) {
  state = State::CallingLiar;

  // Show all player's dice
  for (const auto player : players) player->dice.show();
  // wait for some amount of time
  // print out who was correct

  auto& promise = Global::get().promise.add();
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

    state = players.size() == 1 ? State::Finish : State::Ongoing;
    return true;
  };

#ifdef DEBUG
  promise.sleep(1000).then(judge).sleep(1000).then(reset);
#else
  promise.sleep(3000).then(judge).sleep(2000).then(reset);
#endif
}

void GameScreen::removeLosers() {
  int id = 0;
  auto it = players.begin();
  while (it != players.end()) {
    if ((*it)->dice.getDice().size() == 0) {
      players.erase(it);
      if (id < turn) turn--;
      break;
    }
    id++;
    it++;
  }
}