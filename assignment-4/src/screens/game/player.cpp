#include "screens/game/player.h"

Player::Player(const GameScreen* const game, Dice dice, const std::string& name,
               uint type)
    : name(name), type(type), game(game), dice(dice) {}
Player::~Player() {}

void Player::draw() const { dice.draw(); }