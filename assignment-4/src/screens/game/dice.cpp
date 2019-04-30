#include "screens/game/dice.h"
#include <stdlib.h>
#include <cmath>
#include "widget/explosion.h"

Dice::Dice(const Vector2f& position, double theta, unsigned int numDice, int g)
    : dice(numDice), gap(g) {
  roll();
  positionDie(position, theta);
}

void Dice::roll() {
  for (auto& d : dice) {
    d.set(rand() % 6);
  }
}

void Dice::positionDie(const Vector2f& start, double theta = 0) {
  double dx = cos(theta) * gap;
  double dy = sin(theta) * gap;
  for (uint i = 0; i < dice.size(); i++) {
    dice[i].set(Vector2f(i * dx + start[0], i * dy + start[1]));
  }
}

void Dice::draw() const {
  for (const auto& d : dice) {
    d.draw();
  }
}

void Dice::show() {
  for (auto& d : dice) {
    d.set(Die::State::visible);
  }
}

void Dice::hide() {
  for (auto& d : dice) {
    d.set(Die::State::hidden);
  }
}

void Dice::remove() {
  if (dice.empty()) return;
  const auto& die = dice[dice.size() - 1];
  explosion = Global::get().widget.create<Explosion>(die.getPos());
  dice.pop_back();
  explosion->show()();
}

const std::vector<Die>& Dice::getDice() const { return dice; }

void Dice::set(const std::vector<int>& faces) {
  show();
  auto it = dice.begin();
  for (size_t i = 0; i < faces.size(); i++) {
    dice[i].set(faces[i]);
    it++;
  }
  if (it != dice.end()) {
    dice.erase(it);
  }
}