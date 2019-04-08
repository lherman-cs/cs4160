#pragma once
#include <SDL2/SDL.h>        // for Uint32
#include "core/interface.h"  // for drawable
#include "util/vector2f.h"   // for vector

class Bet : public Drawable {
 public:
  enum Type { Quantity, Face };
  struct Value {
    int quantity;
    int face;
  };
  Bet(const Vector2f& pos = Vector2f(0, 0), int gap = 0,
      const Value& val = {0, 1});
  void draw() const;
  Bet& increment(Type);
  Bet& decrement(Type);
  // bad structure, we should get this from game automatically
  void updateDice(int);

 private:
  Value last;
  Value current;
  int diceOnTable = 25;
  const int numFaces = 6;
  int gap;
  Vector2f position;
  Bet& validate();
};