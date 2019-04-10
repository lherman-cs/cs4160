#pragma once
#include <SDL2/SDL.h>           // for Uint32
#include "core/interface.h"     // for drawable
#include "screens/game/game.h"  // for GameScreen
#include "util/ioMod.h"
#include "util/vector2f.h"  // for vector

class Bet : public Drawable {
 public:
  enum Type { Quantity, Face };
  struct Value {
    int quantity;
    int face;
  };

  Bet(const GameScreen* const, const Vector2f& pos = Vector2f(0, 0),
      int gap = 0, const Value& val = {0, 1});
  void draw() const;
  Bet& increment(Type);
  Bet& decrement(Type);
  int getQuantity() const;
  int getType() const;
  // bad structure, we should get this from game automatically
  void updateDice(int);

 private:
  Value last;
  Value current;
  const int numFaces = 6;
  int gap;
  Vector2f position;
  Bet& validate();
  const GameScreen* const game;
  IoMod menuWriter{60};
};