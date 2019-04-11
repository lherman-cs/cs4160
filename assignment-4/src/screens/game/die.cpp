#include "screens/game/die.h"
#include <algorithm>

Die::Die() {}

Die::Die(const Vector2f& pos, State s, int v)
    : state(s), value(v), position(pos) {}

void Die::draw() const {
  switch (state) {
    case State::hidden:
      hide.draw(position);
      break;
    case State::invisible:
      gone.draw(position);
      break;
    case State::visible:
      (*faces)[value]->draw(position);
      break;
  }
}

Die& Die::select() {
  faces = &highlightedDice;
  return *this;
}
Die& Die::deselect() {
  faces = &unhighlightedDice;
  return *this;
}

Die& Die::set(const Vector2f& pos) {
  position = pos;
  return *this;
}
Die& Die::set(const Die::State s) {
  if (s != State::invisible) {
    state = s;
  }
  return *this;
}
Die& Die::set(int v) {
  v = abs(v);
  v %= 6;
  value = v;
  return *this;
}

int Die::getValue() const { return value; }

int Die::getSize() { return 32; }