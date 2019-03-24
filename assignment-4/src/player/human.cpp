#include "player/human.h"

Human::Human(const GameScreen& game, std::string name) : Player(game, name) {}
Human::~Human() {}

void Human::onKeyDown(const Uint8* const keystate) {}