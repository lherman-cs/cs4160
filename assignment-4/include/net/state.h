#pragma once
#include <unordered_map>
#include <vector>
#include "net/player.h"
#include "screens/game/bet.h"
#include "screens/game/const.h"
#include "screens/game/human.h"
#include "screens/game/player.h"

// for holding game state
class State {
 public:
  inline int getNumDice() const { return numDice; }

 protected:
  friend NetGameScreen;
  State(const int&);
  void updateState(const std::unordered_map<std::string, std::string>&);
  void setPlayers(const std::vector<std::string>&);

  std::shared_ptr<Bet> bet;
  int turn{0};
  int round{0};
  int numDice{25};
  std::vector<std::shared_ptr<Player>> players{};
};