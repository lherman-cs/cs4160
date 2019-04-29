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
// #include "screens/help.h"
// #include "screens/intro.h"
#include "util/ioMod.h"
#include "widget/loading.h"

using namespace std::placeholders;

// game screen
//  accepts face and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of face and quantity

NetGameScreen::NetGameScreen(std::shared_ptr<TCP> session, int index,
                             int difficulty)
    : session(session), index(index) {
  (void)difficulty;
}

NetGameScreen::~NetGameScreen() {}

void NetGameScreen::onKeyDown(const Uint8* const keystate) {
  (void)keystate;
  if (state == Status::Initalizing) return;
}

void NetGameScreen::draw() const {
  if (state == Status::Initalizing) {
    // draw loading bar
  }
}

void NetGameScreen::update(Uint32 ticks) {
  (void)ticks;
  // do nothing when offline
  if (session->isOffline()) return;

  // Create appropriate data structures
  net::message msg;
  bool ready = session->read(msg);
  if (!ready) return;

  auto type = msg["type"];
  if (type == "roll") {
    // Set dice
    for (int i = 0; i < gameData.players.size(); i++) {
      auto faces = toint(msg[std::to_string(i)]);
      gameData.players[i]->dice.set(faces);
    }
  } else if (type == "state") {
    // Set state
    gameData.updateState(msg);
  } else {
    return;
  }

  // // Handle player going offline
  // if (session->isOffline()) {
  //   auto loading = Global::get().widget.create<Loading>("You're offline");
  //   auto redirecting = Global::get().widget.create<Loading>("Going back");
  //   auto goBack = []() {
  //     Global::get().navigator.pop();
  //     return true;
  //   };

  //   auto& promise = Global::get().promise.add();
  //   promise.then(loading->show())
  //       .sleep(2000)
  //       .then(loading->dismiss())
  //       .then(redirecting->show())
  //       .sleep(500)
  //       .then(goBack)
  //       .sleep(500)
  //       .then(redirecting->dismiss());
  // }
}