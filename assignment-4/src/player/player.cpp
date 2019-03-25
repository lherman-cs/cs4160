#include "player/player.h"

Player::Player(const GameScreen* const game, const std::string& name, uint type)
    : name(name), type(type), game(game) {}
Player::~Player() {}
