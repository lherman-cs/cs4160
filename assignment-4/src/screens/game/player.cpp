#include "screens/game/player.h"

Player::Player(const State* const s, Dice dice, const std::string& name,
               uint type)
    : name(name), type(type), dice(dice), state(s) {}
Player::~Player() {}

void Player::draw() const { dice.draw(); }