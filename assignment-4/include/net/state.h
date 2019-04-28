#include <unordered_map>
#include <vector>
#include "screens/game/bet.h"
#include "screens/game/const.h"
#include "screens/game/player.h"

// for holding game state
class State {
 public:
  State(const GameScreen* const g, const int&);
  void updateState(const std::unordered_map<std::string, std::string>&);
  void setPlayers(const std::vector<std::string>&);

  std::shared_ptr<Bet> bet;
  int turn{0};
  int round{0};
  int numDice{25};
  std::vector<std::shared_ptr<Player>> players{};
};