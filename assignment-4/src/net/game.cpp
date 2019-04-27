#include "screens/net/game.h"
// #include <algorithm>
// #include <functional>
#include <iostream>
// #include <sstream>
// #include "global/global.h"
// #include "screens/game/bot.h"
#include "screens/game/dice.h"
#include "screens/game/human.h"
#inlclude "screens/net/player.h"
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
  std::vector<std::string> n = [ "Me", "p1", "p2", "p3", "p4" ];

  // wait for roll message from the server
  //  has the dice faces

  for (int i = 0; i < NUMPLAYERS; i++) {
    this->players.emplace_back(
        i == index ? std::make_shared<Human>(
                         this, Dice(dicePos[i].first, dicePos[i].second), n[i])
                   : std::make_shared<NetworkedPlayer>(
                         this, Dice(dicePos[i].first, dicePos[i].second), n[i]))
  }
}

NetGameScreen::~NetGameScreen() {}

// parses a comma-seperated string into array of strings
const std::vector<std::string> tostr(const std::string& in) {
  stringstream ss(in);
  vector<std::string> result;

  while (ss.good()) {
    std::string substr;
    getline(ss, substr, ',');
    result.push_back(substr);
  }
}

// parses a comma-seperated string into array on ints
const std::vector<int> toint(std::string) {
  stringstream ss(in);
  vector<std::string> result;

  while (ss.good()) {
    std::string substr;
    getline(ss, substr, ',');
    result.push_back(std::stoi(substr));
  }
}

// called when we recieved a state from the server
void NetGameScreen::updateState(
    const std::unordered_map<std::string, std::string>& update) {
  // players
  auto p = tostr(update["players"]);
  for (int i = 0; i < NUMPLAYERS; i++) {
    players[i]->name = p[i];
  }

  // turn
  turn = std::stoi(update["turn"]);

  // round
  round = std::stoi(update["round"]);

  // num_dices
  diceOnTable = std::stoi(update["num_dices"]);
}