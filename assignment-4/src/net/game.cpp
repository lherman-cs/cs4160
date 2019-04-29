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

NetGameScreen::NetGameScreen(int difficulty) {
  (void)difficulty;
  auto gameSession = std::make_shared<TCP>();
}

NetGameScreen::~NetGameScreen() {}

void NetGameScreen::onKeyDown(const Uint8* const keystate) { (void)keystate; }

void NetGameScreen::draw() const {}

void NetGameScreen::update(Uint32 ticks) {
  // capture offline
  if (session->isOffline()) return;
  if (state == Initalizing) {
    // Create appropriate data structures

    auto& promise = Global::get().promise.add();
    auto resp = std::make_shared<net::message>();

    // Get dice

    auto getDice = [=]() {
      if (session->isOffline()) {
        // joining = false;
        return true;
      }
      bool done = session->read(*resp);
      if (!done) return false;

      if (resp->find("error") != resp->end() ||
          resp->find("roll") != resp->end()) {
        // joining = false;
        return true;
      }

      auto& navigator = Global::get().navigator;
      navigator.pop();
      // navigator.push<RoomScreen>(session, 0, false);
      return true;
    };

    promise.then(getDice);

    // for (auto player : gameData->players) {
    //   player->
    // }

    // Get state

    auto getState = [=]() {
      if (session->isOffline()) {
        // joining = false;
        return true;
      }
      bool done = session->read(*resp);
      if (!done) return false;

      if (resp->find("error") != resp->end()) {
        // joining = false;
        return true;
      }

      auto& navigator = Global::get().navigator;
      navigator.pop();
      // navigator.push<RoomScreen>(session, 0, false);
      return true;
    };

    promise.then(getState);

    // Check if done
    state = Ongoing;
    return;
  }

  // Handle player going offline
  if (session->isOffline()) {
    auto loading = Global::get().widget.create<Loading>("You're offline");
    auto redirecting = Global::get().widget.create<Loading>("Going back");
    auto goBack = []() {
      Global::get().navigator.pop();
      return true;
    };

    auto& promise = Global::get().promise.add();
    promise.then(loading->show())
        .sleep(2000)
        .then(loading->dismiss())
        .then(redirecting->show())
        .sleep(500)
        .then(goBack)
        .sleep(500)
        .then(redirecting->dismiss());
  }
}