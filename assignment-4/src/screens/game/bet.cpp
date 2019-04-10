#include "screens/game/bet.h"

Bet::Bet(const GameScreen* const g, const Vector2f& pos, int ga, const Value& l,
         const Value& c)
    : game(g), position(pos), gap(ga), last(l), current(c) {}

void Bet::draw() const {
  menuWriter.writeText(std::to_string(current.quantity), 825, 30, *textColor);
  die.draw();
  // std::cout << current.quantity << ", " << current.face << std::endl;
}

void Bet::setSelectable(bool b) {
  if (b) {
    if (dieSelected)
      die.select();
    else
      textColor = &hoverColor;
  } else {
    if (dieSelected)
      die.deselect();
    else
      textColor = &normalColor;
  }
}
void Bet::selectDie(bool b) {
  dieSelected = b;
  b ? die.select() : die.deselect();
  textColor = b ? &normalColor : &hoverColor;
}

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
  // update die rendering
  die.set(current.face - 1);
  return *this;
}

Bet& Bet::decrement(Type t) {
  switch (t) {
    case Quantity:
      current.quantity -= 1;
      break;

    case Face:
      current.face -= 1;
      break;
  }
  validate();
  // update die rendering
  die.set(current.face - 1);
  return *this;
}

/**
 * If **valid**, returns `true`. Else, current will be **adjusted** to be valid
 * and return `false`.
 */
bool Bet::validate() {
  auto _quantity = current.quantity;
  auto _face = current.face;

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

  return _quantity == current.quantity && _face == current.face;
}