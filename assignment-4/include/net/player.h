#pragma once
#include "screens/game/player.h"

// all logic for a networked player is done elsewhere,
//      we have an empty class to hold the name
class NetworkedPlayer : public Player {
 public:
  NetworkedPlayer(const GameScreen *const game, Dice d, std::string name);
  virtual ~NetworkedPlayer();
};