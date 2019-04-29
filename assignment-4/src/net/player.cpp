#include "net/player.h"

NetworkedPlayer::NetworkedPlayer(const State* const s, Dice d, std::string name)
    : Player(s, d, name, 0) {}

NetworkedPlayer::~NetworkedPlayer() {}