#include "player/player.h"

Player::Player(const GameScreen& game, const std::string& name)
    : name(name), game(game) {}
Player::~Player() {}
