#include "player/bot.h"

Bot::Bot(const GameScreen* const game, int id) : Player(game, "bot " + id, 1) {}
Bot::~Bot() {}
