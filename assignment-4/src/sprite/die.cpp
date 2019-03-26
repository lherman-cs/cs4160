#include "sprite/die.h"

Die::Die() {}

Die::Die(const Vector2f& pos, State s, int v)
    : state(s), value(v), position(pos) {}

void Die::draw() const {
  switch (state) {
    case State::hidden:
      hide.draw();
      break;
    case State::invisible:
      gone.draw();
      break;
    case State::visible:
      faces[value]->draw(position);
      break;
  }
}

void Die::update(Uint32 ticks) { (void)ticks; }

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
  value = v;
  return *this;
}

int Die::getSize() { return 32; }