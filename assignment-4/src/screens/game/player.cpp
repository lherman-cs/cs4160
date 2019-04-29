#include "screens/game/player.h"

Player::Player(const State* const s, Dice dice, const std::string& name,
               uint type)
    : state(s), name(name), type(type), dice(dice) {}
Player::~Player() {}

void Player::draw() const { dice.draw(); }