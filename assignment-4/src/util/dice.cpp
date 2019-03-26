#include "util/dice.h"
#include <stdlib.h>
#include <cmath>

Dice::Dice(long unsigned int numDice, int g) : dice({numDice}), gap(g) {
  roll();
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