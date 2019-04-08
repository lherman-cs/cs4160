// Bet class, a shared drawable object which is responsible for maintaining
// bet validity and drawing the bet, managing highlighted states

#pragma once
#include <SDL2/SDL.h>        // for Uint32
#include "core/interface.h"  // for drawable
#include "util/vector2f.h"   // for vector

class Bet : public Drawable {
 public:
  enum Type { Quantity, Face };
  Bet(const Vector2f& pos = Vector2f(0, 0), int gap = 0,
      const Vector2f& val = Vector2f(0, 1));
  void draw() const;
  Bet& increment(Type);
  Bet& decrement(Type);

 private:
  Vector2f value = Vector2f(0, 1);
  int gap = 0;
  Vector2f position = Vector2f(0, 0);
  Bet& validate();
};