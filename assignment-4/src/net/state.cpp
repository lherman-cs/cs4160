#include "net/state.h"
#include "net/encoder.h"
#include "screens/game/dice.h"

State::State(const int& index) {
  // initialize players
  for (int i = 0; i < NUMPLAYERS; i++) {
    if (i == index)
      players.emplace_back(std::make_shared<Human>(
          this, Dice(dicePos[i].first, dicePos[i].second), ""));
    else
      players.emplace_back(std::make_shared<NetworkedPlayer>(
          this, Dice(dicePos[i].first, dicePos[i].second), ""));
  }
}

State::State() {
#ifdef DEBUG
  // if we are in debug mode, we have bots play against eachother
  int start = 0;
#else
  // otherwise, have an interactive game (human playing against bots)
  int start = 1;
  auto human = std::make_shared<Human>(
      this, Dice(dicePos[0].first, dicePos[0].second), "Human");
  this->players.emplace_back(human);
#endif

  for (int id = start; id < NUMPLAYERS; id++) {
    auto bot = std::make_shared<Bot>(
        this, Dice(dicePos[id].first, dicePos[id].second), id);
    this->players.emplace_back(bot);
  }
}

void State::setPlayers(const std::vector<std::string>& p) {
  for (unsigned int i = 0; i < p.size(); i++) {
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