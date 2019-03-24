#include "player/player.h"

Player::Player(const std::string &name) : name(name) {}

Player::~Player() { emit<SetBetEvent>(SetBetEvent(30, 10)); }