#include "player/player.h"

Player::Player(const GameScreen* const game, const std::string& name, uint type)
    : name(name), game(game), type(type) {}
Player::~Player() {}
