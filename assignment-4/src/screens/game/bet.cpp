#include "screens/game/bet.h"

Bet::Bet(const State* const s, const Vector2f& pos, const Value& l,
         const Value& c)
    : state(s), position(pos), last(l), current(c) {}

void Bet::draw() const {
  menuWriter.writeText(std::to_string(current.quantity), 825, 30, *textColor);
  die.draw();
  // std::cout << current.quantity << ", " << current.face << std::endl;
}

void Bet::setSelectable(bool b) {
  if (!b) {
    die.deselect();
    textColor = &normalColor;
    return;
  }

  if (dieSelected)
    die.select();
  else
    textColor = &hoverColor;
}

void Bet::select(Type t) {
  dieSelected = t == Face ? true : false;
  dieSelected ? die.select() : die.deselect();
  textColor = dieSelected ? &normalColor : &hoverColor;
}

void Bet::submit() {
  last = current;
  rand() % 100 < 70 ? current.quantity++ : current.face++;
  validate();
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
  select(t);
  // update die rendering
  die.set(current.face);
  return *this;
}

Bet& Bet::decrement(Type t) {
  switch (t) {
    case Quantity:
      current.quantity--;
      break;

    case Face:
      current.face--;
      break;
  }
  validate();
  select(t);
  // update die rendering
  die.set(current.face);
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
  else if (current.quantity > state->getNumDice())
    current.quantity = state->getNumDice();
  else if (current.quantity <= 0)
    current.quantity = 1;

  if (current.face < last.face)
    current.face = last.face;
  else if (current.face > numFaces - 1)
    current.face = numFaces - 1;

  if (current.quantity == last.quantity && current.face == last.face) {
    if (current.quantity == state->getNumDice())
      current.face++;
    else
      current.quantity++;
  }

  return _quantity == current.quantity && _face == current.face;
}

Bet::Value Bet::getLast() const { return last; }
Bet::Value Bet::getCurr() const { return current; }

void Bet::reset() {
  last = {0, 0};
  current = last;
  increment(Quantity);
}

void Bet::set(const Value& v) {
  last = v;
  current = v;
  die.set(current.face);
  validate();
}