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

void Bet::updateDice(int num) : diceOnTable(num) {}

Bet& Bet::validate() {
  if (current.quantity < last.quantity)
    current.quantity = last.quantity;
  else if (current.quantity > diceOnTable)
    current.quantity = diceOnTable;
  else if (current.quantity <= 0)
    current.quantity = 1;

  if (current.type < last.type)
    current.type = last.type;
  else if (current.type > numFaces)
    current.type = numFaces;

  if (current.quantity == last.quantity && current.type == last.type) {
    if (current.quantity == diceOnTable)
      current.type++;
    else
      current.quantity++;
  }
}