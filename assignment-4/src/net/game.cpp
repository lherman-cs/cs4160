#include "net/game.h"
// #include <algorithm>
// #include <functional>
#include <iostream>
// #include <sstream>
// #include "global/global.h"
// #include "screens/game/bot.h"
#include "net/encoder.h"
#include "net/player.h"
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

NetGameScreen::NetGameScreen(std::shared_ptr<TCP> session, int index,
                             int difficulty)
    : session(session), index(index), gameData(index) {
  (void)difficulty;
  std::cout << "Net Game Class init" << std::endl;
}

NetGameScreen::~NetGameScreen() {}

void NetGameScreen::onKeyDown(const Uint8* const keystate) {
  (void)keystate;
  if (state == Status::Initializing || state == Status::OnCall ||
      state == Status::OnRoll)
    return;

  if (keystate[SDL_SCANCODE_L]) {
    // Notify server
    auto& promise = Global::get().promise.add();
    auto msg = net::gameCall();
    promise.then([=]() { return session->write(*msg); });
  }

  auto turn = gameData.turn;
  // Dont allow user input when it's not their turn (except for call liar)
  if (turn != index) return;

  if (keystate[SDL_SCANCODE_H]) {
    navigator.push<HelpScreen>();
  }

  if (keystate[SDL_SCANCODE_R]) {
    navigator.reset();
    navigator.push<IntroScreen>();
    return;
  }

  bool done = gameData.players[index]->decide(keystate, gameData.bet);
  if (done) {
    std::cout << "ENTER BET" << std::endl;
    auto& promise = Global::get().promise.add();
    auto bet = gameData.bet->getCurr();
    auto msg = net::gameBet(bet.quantity, bet.face - 1);
    promise.then([=]() { return session->write(*msg); });
  }
}

void NetGameScreen::draw() const {
  background.draw();
  // Draw player's dice
  for (const auto& player : gameData.players) player->draw();

  // Draw current bet
  gameData.bet->draw();

  // Draw round/turn number
  int ystart = 705;
  int xstart = 50;
  int xstep = 110;

  menuWriter.writeText(std::to_string(gameData.round), xstart + xstep, ystart,
                       secondaryColor);

  if (state == Status::Initializing) {
    // draw loading bar
    return;
  }
}

void NetGameScreen::update(Uint32 ticks) {
  (void)ticks;
  if (session->isOffline()) {
    // TODO! Give a little bit animation here
    navigator.reset();
    navigator.push<IntroScreen>();
    return;
  }

  // Create appropriate data structures
  net::message msg;
  bool ready = session->read(msg);
  if (!ready) return;

  std::string type = msg["type"];
  if (type == "roll") {
    // Set dice to be values recieved from the server
    state = Status::OnRoll;
    std::cout << "Got rolling dice message" << std::endl;
    for (unsigned int i = 0; i < gameData.players.size(); i++) {
      auto id = std::to_string(i);
      auto faces = toVecInt(msg[id]);
      gameData.players[i]->dice.set(faces);
    }
    // Set the dice to be hidden (unless it yours)
    for (unsigned int i = 0; i < gameData.players.size(); i++) {
      if (i != index) gameData.players[i]->dice.hide();
    }
  } else if (type == "state") {
    // Set state
    std::cout << "Got state message" << std::endl;
    state = Status::Ongoing;
    gameData.updateState(msg);
  } else if (type == "call") {
    std::cout << "Got call message" << std::endl;
    // update internal state so that we can render a loading bar
    state = Status::OnCall;
    // show all the dice on the table
    for (const auto& player : gameData.players) {
      player->dice.show();
    }
  } else if (type == "finish") {
    std::cout << "Got finish message" << std::endl;
    state = Status::OnFinish;
    return;
  }
}