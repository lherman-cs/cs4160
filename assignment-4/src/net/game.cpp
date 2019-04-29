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
}

NetGameScreen::~NetGameScreen() {}

void NetGameScreen::onKeyDown(const Uint8* const keystate) {
  (void)keystate;
  if (state == Status::Initializing) return;

  auto turn = gameData.turn;

  if (keystate[SDL_SCANCODE_L]) {
    // Notify server
    auto& promise = Global::get().promise.add();
    auto msg = net::gameCall();
    promise.then([=]() { return session->write(*msg); });
  }

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

  if (state == Status::OnCall || state == Status::OnRoll) return;
}

void NetGameScreen::draw() const {
  background.draw();
  for (const auto& player : gameData.players) player->draw();

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
    // Set dice
    state = Status::OnRoll;
    for (unsigned int i = 0; i < gameData.players.size(); i++) {
      auto id = std::to_string(i);
      auto faces = toVecInt(msg[id]);
      gameData.players[i]->dice.set(faces);
    }
  } else if (type == "state") {
    // Set state
    state = Status::Ongoing;
    gameData.updateState(msg);
  } else if (type == "call") {
    // update internal state so that we can render a loading bar
    state = Status::OnCall;
  } else if (type == "finish") {
    state = Status::OnFinish;
    return;
  }
}