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
      column(0),
      players(players),
      lastBet({0, 1}),
      currBet({1, 1}),
      menuWriter(70, {88, 53, 31, 255}),
      navigator(Navigator::getInstance()) {
  // auto onSetBet = std::bind(&GameScreen::onSetBet, this, _1);
  // auto onCallLiar = std::bind(&GameScreen::onCallLiar, this, _1);

  // for (int id = 1; id <= bots; id++) {
  //   auto bot = std::make_unique<Bot>(*this, id);
  //   bot->subscribe<GameEvent::SetBet>(onSetBet);
  //   bot->subscribe<GameEvent::CallLiar>(onCallLiar);
  //   this->players.emplace_back(std::make_unique<Bot>(id));
  // }
}
void GameScreen::incType() {
  if (currBet.type == 6) {
    if (currBet.quantity > lastBet.quantity) {
      currBet.type = lastBet.type;
    } else {
      if (lastBet.type == 6) {
        incQuan();
      } else {
        if (currBet.quantity > lastBet.quantity)
          currBet.type = lastBet.type;
        else
          currBet.type = lastBet.type + 1;
      }
    }
  } else {
    currBet.type++;
  }
}

void GameScreen::incQuan() {
  if (currBet.quantity < diceOnTable) {
    currBet.quantity++;
  } else {
    if (lastBet.quantity == diceOnTable) {
      incType();
    } else {
      if (currBet.type > lastBet.type) {
        currBet.quantity = lastBet.quantity;
      } else
        currBet.quantity = lastBet.quantity + 1;
    }
  }
}
void GameScreen::decType() {
  if (currBet.type == lastBet.type) {
    return;
  }
  if (currBet.quantity > lastBet.quantity) {
    currBet.type--;
    return;
  }
}
void GameScreen::decQuan() {
  if (currBet.quantity == lastBet.quantity) {
    return;
  }
  if (currBet.type > lastBet.type) {
    currBet.quantity--;
    return;
  }
}
GameScreen::~GameScreen() {}

void GameScreen::onCallLiar(const GameEvent::CallLiar& e) { (void)e; }

void GameScreen::onSetBet(const GameEvent::SetBet& e) { (void)e; }

void GameScreen::onKeyDown(const Uint8* const keystate) {
  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<HelpScreen>();
  }
  // Move Choice
  if (keystate[SDL_SCANCODE_B]) {
  }
  if (keystate[SDL_SCANCODE_L]) {
  }

  // Selection

  // Move later to different section, ineffiecient but ok for now
  // needs to happen every turn
  // State validations and assertions
  {
    if (lastBet.quantity == diceOnTable && lastBet.type == 6) {
      // immediately call liar
    }
    if (lastBet.quantity == currBet.quantity && lastBet.type == currBet.type) {
      currBet.quantity++;
    }
  }

  if (keystate[SDL_SCANCODE_W]) {
    switch (column) {
      case Column::type:
        incType();
        break;
      case Column::quantity:
        incQuan();
        break;
    }
  }
  if (keystate[SDL_SCANCODE_S]) {
    switch (column) {
      case Column::type:
        decType();
        break;
      case Column::quantity:
        decQuan();
        break;
    }
  }
  if (keystate[SDL_SCANCODE_A]) {
    column--;
    if (column < 0) column = 1;
  }
  if (keystate[SDL_SCANCODE_D]) {
    column = (column + 1) % 2;
  }
}

void GameScreen::draw() const { background.draw(); }

void GameScreen::update(Uint32 ticks) { (void)ticks; }