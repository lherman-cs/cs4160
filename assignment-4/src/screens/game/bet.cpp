#include "screens/game/bet.h"

Bet::Bet(const GameScreen* const g, const Vector2f& pos, int ga,
         const Value& val)
    : game(g), position(pos), gap(ga), last(val), current(val) {}
void Bet::draw() const {}

Bet& Bet::increment(Type t) {
  switch (t) {
    case Quantity:
      current.quantity++;
      break;

    case Face:
      current.face++;
      break;
  }
  validate();
}

Bet& Bet::decrement(Type t) {
  switch (t) {
    case Quantity:
      current = last;
      current.quantity += 1;
      break;

    case Face:
      current = last;
      current.face += 1;
      break;
  }
  validate();
}

// should we call increment/decrement here?
Bet& Bet::validate() {
  if (current.quantity < last.quantity)
    current.quantity = last.quantity;
  else if (current.quantity > game->getNumDice())
    current.quantity = game->getNumDice();
  else if (current.quantity <= 0)
    current.quantity = 1;

  if (current.face < last.face)
    current.face = last.face;
  else if (current.face > numFaces)
    current.face = numFaces;

  if (current.quantity == last.quantity && current.face == last.face) {
    if (current.quantity == game->getNumDice())
      current.face++;
    else
      current.quantity++;
  }
}