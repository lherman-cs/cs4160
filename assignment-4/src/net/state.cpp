#include "net/state.h"
#include "net/encoder.h"
#include "screens/game/dice.h"

State::State(const GameScreen* const g, const int& index)
    : bet(std::make_shared<Bet>(g)) {
  int NUMPLAYERS = 5;
  // initialize players
  for (int i = 0; i < NUMPLAYERS; i++) {
    if (i == index)
      players.emplace_back(std::make_shared<Human>(
          g, Dice(dicePos[i].first, dicePos[i].second), ""));
    else
      players.emplace_back(std::make_shared<NetworkedPlayer>(
          g, Dice(dicePos[i].first, dicePos[i].second), ""));
  }
}

void State::setPlayers(const std::vector<std::string>& p) {
  for (int i = 0; i < p.size(); i++) {
    players[i]->name = p[i];
  }
}

// called when we recieved a state from the server
void State::updateState(
    const std::unordered_map<std::string, std::string>& update) {
  // players
  setPlayers(tostr(update.at("players")));

  // turn
  turn = std::stoi(update.at("turn"));

  // round
  round = std::stoi(update.at("round"));

  // num_dices
  numDice = std::stoi(update.at("num_dices"));
}