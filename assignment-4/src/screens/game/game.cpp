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
#include "screens/intro.h"
#include "util/ioMod.h"
#include "widget/loading.h"

using namespace std::placeholders;

// game screen
//  accepts face and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of face and quantity

GameScreen::GameScreen(int difficulty) { (void)difficulty; }

GameScreen::~GameScreen() {}

// Callback for player turn completion.
void GameScreen::onDone() {
  state.turn = (state.turn + 1) % state.players.size();
  state.round++;

  // Allows each bot to get an equal chance of calling liar first, prevents
  //   first bot from always getting to go first
  std::vector<std::shared_ptr<Player>> temp = state.players;
  std::shuffle(
      std::begin(temp), std::end(temp),
      std::default_random_engine(
          std::chrono::system_clock::now().time_since_epoch().count()));
  for (auto player : temp) {
    if (player->callLiar(state.bet)) return onCallLiar(player);
  }

  status = Status::TurnTransition;
  auto& promise = Global::get().promise.add();
  auto loading = Global::get().widget.create<Loading>(
      state.players[state.turn]->name + "'s turn");

  promise.then(loading->show())
      .sleep(1300)
      .then(loading->dismiss())
      .then([&]() -> bool {
        status = Status::Ongoing;
        return true;
      });
}

int GameScreen::getNumDice() const { return state.getNumDice(); }

void GameScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<HelpScreen>();
  }

  if (keystate[SDL_SCANCODE_R]) {
    navigator.reset();
    navigator.push<IntroScreen>();
    return;
  }

  if (status == Status::CallingLiar || status == Status::Finish) return;

  // TODO! check if there's a first bet

  for (auto player : state.players) {
    if (player->callLiar(keystate, state.bet)) return onCallLiar(player);
  }

  bool done = state.players[state.turn]->decide(keystate, state.bet);

  if (done) onDone();
}

void GameScreen::update(Uint32 ticks) {
  if (status == Status::CallingLiar || status == Status::Finish ||
      status == Status::TurnTransition)
    return;

  bool done = state.players[state.turn]->decide(ticks, state.bet);
  if (done) onDone();
}

void GameScreen::draw() const {
  background.draw();

  // Draw bet
  state.bet->draw();

  // Draw players and dice on table
  for (const auto player : state.players) player->draw();

  // Draw round/turn number
  int ystart = 705;
  int xstart = 50;
  int xstep = 110;

  menuWriter.writeText(std::to_string(state.round), xstart + xstep, ystart,
                       secondaryColor);

  if (status == Status::TurnTransition) return;

  if (status == Status::Ongoing) {
    auto player = state.players[state.turn];
    if (player->type == 1) {
      state.bet->setSelectable(false);
      // If bot, renders a loading text saying that the bot is thinking.
      auto loadingText = player->name + " is thinking...";
      loadingWriter.writeText(loadingText, 680, 720, secondaryColor);
    } else {
      state.bet->setSelectable(true);
      // Otherwise, notify the human that it is their turn
      loadingWriter.writeText("Your turn", 770, 720, secondaryColor);
    }
  } else if (status == CallingLiar) {
    loadingWriter.writeText("Judging...", 770, 720, secondaryColor);
  } else {
    const auto finishText = state.players[0]->name + " won!";
    loadingWriter.writeText(finishText, 750, 720, secondaryColor);
  }
}

void GameScreen::onCallLiar(std::shared_ptr<Player> caller) {
  status = Status::CallingLiar;

  // Show all player's dice
  for (const auto player : state.players) player->dice.show();
  // wait for some amount of time
  // print out who was correct

  auto& promise = Global::get().promise.add();
  auto judge = [&, caller]() -> bool {
    // Sums all dice
    int sums[6] = {0};
    for (const auto player : state.players)
      for (const Die die : player->dice.getDice()) sums[die.getValue()]++;

    // Compares to last bet
    if (sums[state.bet->getLast().face] < state.bet->getLast().quantity) {
      // The better lied (the caller was right)
      int pId = (state.turn - 1) % NUMPLAYERS;
      if (pId == -1) pId = state.players.size() - 1;
      unsigned int playerId = pId;
      state.players[playerId]->dice.remove();
    } else {
      // The better told the truth (the caller was wrong)
      caller->dice.remove();
    }
    return true;
  };
  auto reset = [&]() -> bool {
    for (auto player : state.players) {
      player->dice.roll();
      if (player->type == 1) player->dice.hide();
    }
    state.bet->reset();
    state.numDice--;
    removeLosers();

    status = state.players.size() == 1 ? Status::Finish : Status::Ongoing;
    return true;
  };

#ifdef DEBUG
  promise.sleep(1000).then(judge).sleep(1000).then(reset);
#else
  promise.sleep(3000).then(judge).sleep(2000).then(reset);
#endif
}

void GameScreen::removeLosers() {
  unsigned int id = 0;
  auto it = state.players.begin();
  while (it != state.players.end()) {
    if ((*it)->dice.getDice().size() == 0) {
      state.players.erase(it);
      if (id < state.turn) state.turn--;
      break;
    }
    id++;
    it++;
  }
}