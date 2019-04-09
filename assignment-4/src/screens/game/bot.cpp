#include "screens/game/bot.h"
#include <cmath>
#include <cstdlib>

const double MAX_DELAY = 8;  // 4 seconds
const double MAX_SIGMOID_VAL = 12;
const double MAP_STEP = MAX_SIGMOID_VAL / MAX_DELAY;

// map [0, 4] to [0, 12]
static inline double mapInput(double n) { return n * MAP_STEP; }
static inline double sigmoid(double t) { return 1.0 / (1 + exp(-1 * (t - 6))); }

Bot::Bot(const GameScreen* const game, int id)
    : Player(game, "bot " + std::to_string(id), 1), elapsed(0) {}

Bot::~Bot() {}
bool Bot::decide(Uint32 ticks, std::shared_ptr<Bet> bet) {
  // TODO!
  (void)bet;

  elapsed += ticks;
  auto chance = sigmoid(mapInput(elapsed * 1e-3)) * 100;
  auto decided = rand() % 100 < floor(chance);

  if (decided) {
    // random 5% chance of calling liar
    // compute probability
    elapsed = 0;
    return true;
  }
  return false;
}