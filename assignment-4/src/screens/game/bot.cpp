#include "screens/game/bot.h"
#include <cmath>
#include <cstdlib>

#ifdef DEBUG
const double MAX_DELAY = 1;
#else
const double MAX_DELAY = 8;  // 8 seconds
#endif
const double MAX_SIGMOID_VAL = 12;
const double MAP_STEP = MAX_SIGMOID_VAL / MAX_DELAY;

// map [0, 8] to [0, 12]
static inline double mapInput(double n) { return n * MAP_STEP; }
static inline double sigmoid(double t) { return 1.0 / (1 + exp(-1 * (t - 6))); }
static inline int choose(int n, int k) {
  int res = 1;
  if (k > n - k) k = n - k;
  for (int i = 0; i < k; ++i) {
    res *= (n - i);
    res /= (i + 1);
  }
  return res;
}

Bot::Bot(const State* const s, Dice d, int id)
    : Player(s, d, "bot " + std::to_string(id), 1), elapsed(0) {
  dice.hide();
}

Bot::~Bot() {}
bool Bot::decide(Uint32 ticks, std::shared_ptr<Bet> bet) {
  // TODO!
  (void)bet;

  elapsed += ticks;
  auto chance = sigmoid(mapInput(elapsed * 1e-3)) * 100;
  auto decided = rand() % 100 < floor(chance);

  if (decided) {
    bool move = rand() % 100 < 80 ? false : true;
    if (move)
      rand() % 100 < 80 ? bet->increment(Bet::Quantity)
                        : bet->increment(Bet::Face);

    elapsed = 0;
    bet->submit();
    return true;
  }
  return false;
}

/**
 * callLiar will be called every time after somebody makes a bet
 */
bool Bot::callLiar(const std::shared_ptr<const Bet> bet) {
  int matching = 0;
  for (const Die die : dice.getDice()) {
    matching += bet->getLast().face == die.getValue() ? 1 : 0;
  }
  // we are sure it is not a lie
  if (bet->getLast().quantity <= matching) {
    return false;
  }
  double probability = 1 - (bet->getLast().quantity - matching) /
                               (double)(state->getNumDice() - matching);
  return probability <= .75 ? true : false;
}