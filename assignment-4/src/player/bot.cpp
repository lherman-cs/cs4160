#include "player/bot.h"

Bot::Bot(const GameScreen& game, int id) : Player(game, "bot " + id) {}
Bot::~Bot() {}
