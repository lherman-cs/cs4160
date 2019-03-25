#include "player/bot.h"

Bot::Bot(const GameScreen *const game, int id) : Player(game, "bot " + id, 1) {}
Bot::~Bot() {}
bool Bot::decide(Uint32 ticks, int &quantity, int &type) {
  return true;
  //   // if we have waited long enough
  //   if (decided && ticks - start > 20000) {
  //     decided = false;
  //     return true;
  //   }
  //   // if this is the beginning of our turn, choose a move
  //   if (!decided) {
  //     start = ticks;
  //     move(quantity, type);
  //     decided = true;
  //   }
  //   return false;
}

void Bot::move(int &quantity, int &type) {
  // random 5% chance of calling liar
  // compute probability
}