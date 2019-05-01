#pragma once
#include <unordered_map>
#include <vector>
#include "net/player.h"
#include "screens/game/bet.h"
#include "screens/game/bot.h"
#include "screens/game/const.h"
#include "screens/game/human.h"
#include "screens/game/player.h"
#include "widget/token.h"

// for holding game state
class State {
 public:
  inline int getNumDice() const { return numDice; }

 protected:
  friend NetGameScreen;
  friend GameScreen;
  // used in networked / online / multiplayer games
  State(const int&);
  // used in local games
  State();
  void updateState(const std::unordered_map<std::string, std::string>&);
  void setPlayers(const std::vector<std::string>&);

  std::shared_ptr<Bet> bet{std::make_shared<Bet>(this)};
  unsigned int turn{0};
  unsigned int round{0};
  unsigned int numDice{25};
  std::vector<std::shared_ptr<Player>> players{};
  std::shared_ptr<Token> token{nullptr};

 private:
  static const int NUMPLAYERS = 5;
};