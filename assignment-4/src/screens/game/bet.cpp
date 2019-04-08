#include "screens/game/bet.h"

Bet::Bet(const Vector2f& pos, int g, const Vector2f& val)
    : position(pos), gap(g), value(val) {}

void Bet::draw() const {}

Bet& Bet::increment(Type t) {
  switch (t) {
    case Quantity:
      value += Vector2f(1, 0);
      break;

    case Face:
      value += Vector2f(0, 1);
      break;
  }
  validate();
}

Bet& Bet::decrement(Type t) {
  switch (t) {
    case Quantity:
      value -= Vector2f(1, 0);
      break;

    case Face:
      value -= Vector2f(0, 1);
      break;
  }
  validate();
}

// TODO
Bet& Bet::validate() {}