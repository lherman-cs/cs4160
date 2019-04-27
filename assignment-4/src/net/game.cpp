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
int NUMPLAYERS = 5;

// game screen
//  accepts face and quantity (of dice for betting)
//  responds to 'B' bet, 'L' call liar, and 'H' help
//              ← → ↑ ↓ for selection of face and quantity

NetGameScreen::NetGameScreen(int difficulty)
    : bet(std::make_shared<Bet>(this)) {
  (void)difficulty;

  // wait for start messsage from the server
  //  start message gets list of players and index of you

  // for testing set index
  index = 0;
  // for testing player names
  std::vector<std::string> n = {"Me", "p1", "p2", "p3", "p4"};

  // wait for roll message from the server
  //  has the dice faces

  for (int i = 0; i < NUMPLAYERS; i++) {
    if (i == index)
      this->players.emplace_back(std::make_shared<Human>(
          this, Dice(dicePos[i].first, dicePos[i].second), n[i]));
    else
      this->players.emplace_back(std::make_shared<NetworkedPlayer>(
          this, Dice(dicePos[i].first, dicePos[i].second), n[i]));
  }
}

NetGameScreen::~NetGameScreen() {}