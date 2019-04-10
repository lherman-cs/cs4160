#include "screens/game/bet.h"

Bet::Bet(const GameScreen* const g, const Vector2f& pos, int ga, const Value& l,
         const Value& c)
    : game(g), position(pos), gap(ga), last(l), current(c) {}

void Bet::draw() const {
  if (selectable) {
    menuWriter.writeText(std::to_string(current.quantity), 825, 30, *textColor);
    die.draw();
    return;
  }
  menuWriter.writeText(std::to_string(current.quantity), 825, 30, normalColor);
  die.draw();
  std::cout << current.quantity << ", " << current.face << std::endl;
}

void Bet::setSelectable(bool b) {
  selectable = b;
  b ? die.select() : die.deselect();
}
void Bet::selectDie(bool b) {
  b ? die.select() : die.deselect();
  b ? textColor = &normalColor : textColor = &hoverColor;
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
  return *this;
}

// should we call increment/decrement here?
void Bet::validate() {
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
  die.set(current.face - 1);
}