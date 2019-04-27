#include <unordered_map>
#include <vector>
#include "screens/game/bet.h"
#include "screens/game/player.h"

// for holding game state
class State {
 public:
  void updateState(const std::unordered_map<std::string, std::string>&);
  void setPlayers(const std::vector<std::string>&);

  Bet bet;
  int turn;
  int round;
  int numDice;
  std::vector<std::shared_ptr<Player>> players{};
};