#include "net/state.h"
#include "net/encoder.h"

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