#include "widget/explosion.h"

Explosion::Explosion(Vector2f pos) { explosion.setPosition(pos); }

void Explosion::draw() const {
  if (showing) {
    explosion.draw();
  }
}

void Explosion::update(Uint32 ticks) {
  if (showing) explosion.update(ticks);
}