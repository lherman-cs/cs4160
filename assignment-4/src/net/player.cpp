#include "net/player.h"

NetworkedPlayer::NetworkedPlayer(const GameScreen* const game, Dice d,
                                 std::string name)
    : Player(game, d, name, 0) {}

NetworkedPlayer::~NetworkedPlayer() {}