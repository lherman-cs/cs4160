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
  // std::cout << "Net Game Class init" << std::endl;
}

NetGameScreen::~NetGameScreen() {}

void NetGameScreen::onKeyDown(const Uint8* const keystate) {
  (void)keystate;
  if (state == Status::Initializing || state == Status::OnCall ||
      state == Status::OnRoll || state == Status::OnFinish)
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
    // std::cout << "ENTER BET" << std::endl;
    auto& promise = Global::get().promise.add();
    auto bet = gameData.bet->getLast();
    // std::cout << "SENDING BET!!!!!!!!!!! " << bet.quantity << ", " <<
    // bet.face
    // << std::endl;
    auto msg = net::gameBet(bet.quantity, bet.face);
    promise.then([=]() { return session->write(*msg); });
  }
}

void NetGameScreen::draw() const {
  background.draw();
  // Draw player's dice
  for (const auto& player : gameData.players) player->draw();

  // Draw current bet
  gameData.bet->draw();

  // Draw the token to represent player turn
  gameData.token.draw();

  // Draw round/turn number
  std::string round = "Turn: " + std::to_string(gameData.round);
  menuWriter.writeText(round, 10, 690, secondaryColor);

  if (state == Status::Initializing) {
    // draw loading bar
    return;
  }

  if (state == Status::OnFinish) return;

  auto last = gameData.bet->getLast();
  if (last.quantity != 0) {
    // draw last bet
    auto offset = last.quantity > 9 ? 15 : 0;
    std::string quantity =
        std::to_string(gameData.bet->getLast().quantity) + "x";
    helperWriter.writeText(quantity, 470 - offset, 401, {255, 255, 255, 255});
    helperDie.draw();
  }

  if (state == Status::Ongoing) {
    auto player = gameData.players[gameData.turn];
    // if the player is a networked player/bot
    if (player->type == 1) {
      gameData.bet->setSelectable(false);
      // If bot, renders a loading text saying that the bot is thinking.
      auto loadingText = player->name + " is thinking...";
      loadingWriter.writeText(loadingText, 550, 720, secondaryColor);
    } else {
      gameData.bet->setSelectable(true);
      // Otherwise, notify the human that it is their turn
      loadingWriter.writeText("Your turn", 770, 720, secondaryColor);
    }
  }
}

void NetGameScreen::update(Uint32 ticks) {
  (void)ticks;
  if (state == Status::OnFinish) return;
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
    state = Status::OnRoll;
    // std::cout << "Got rolling dice message" << std::endl;
    // Set dice to be values recieved from the server
    for (unsigned int i = 0; i < gameData.players.size(); i++) {
      auto id = std::to_string(i);
      auto faces = toVecInt(msg[id]);
      auto& dice = gameData.players[i]->dice;
      while (dice.getDice().size() > faces.size()) dice.remove();
      gameData.players[i]->dice.set(faces);
    }
    // Set the dice to be hidden (unless it yours)
    for (unsigned int i = 0; i < gameData.players.size(); i++) {
      if (i != index) gameData.players[i]->dice.hide();
    }
  } else if (type == "state") {
    // Set state
    // std::cout << "Got state message" << std::endl;
    state = Status::Ongoing;
    gameData.updateState(msg);
  } else if (type == "call") {
    // std::cout << "Got call message" << std::endl;
    // update internal state so that we can render a loading bar
    state = Status::OnCall;
    auto liarNotice = Global::get().widget.create<Loading>("Liar Called");
    auto& promise = Global::get().promise.add();
    promise.then(liarNotice->show()).sleep(1400).then(liarNotice->dismiss());
    // show all the dice on the table
    for (const auto& player : gameData.players) {
      player->dice.show();
    }
  } else if (type == "finish") {
    // std::cout << "Got finish message" << std::endl;
    state = Status::OnFinish;
    auto winner = msg["winner"];
    auto loading = Global::get().widget.create<Loading>(winner + " won!", 35);
    auto redirecting = Global::get().widget.create<Loading>("Going home", 35);
    auto goBack = []() {
      auto& navigator = Global::get().navigator;
      navigator.reset();
      navigator.push<IntroScreen>();
      return true;
    };

    auto& promise = Global::get().promise.add();
    Global::get().mixer.background.pause();
    Global::get().mixer.finish.play();
    promise.then(loading->show())
        .sleep(6000)
        .then(loading->dismiss())
        .then(redirecting->show())
        .sleep(1000)
        .then(goBack)
        .sleep(500)
        .then([]() {
          Global::get().mixer.background.resume();
          return true;
        })
        .then(redirecting->dismiss());
  }
  helperDie.set(gameData.bet->getLast().face);
}