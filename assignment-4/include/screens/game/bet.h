#pragma once
#include <SDL2/SDL.h>           // for Uint32
#include "core/interface.h"     // for drawable
#include "screens/game/game.h"  // for GameScreen
#include "util/ioMod.h"         // for drawing
#include "util/vector2f.h"      // for vector

class GameScreen;

class Bet : public Drawable {
 public:
  enum Type { Quantity, Face };
  struct Value {
    int quantity;
    int face;
  };

  Bet(const GameScreen* const, const Vector2f& pos = Vector2f(0, 0),
      int gap = 0, const Value& val = {0, 1});
  Bet(const Bet&) = delete;
  Bet& operator=(const Bet&) = delete;
  void draw() const;
  Bet& increment(Type);
  Bet& decrement(Type);
  int getQuantity() const;
  int getType() const;

 private:
  const GameScreen* const game;
  Vector2f position;
  int gap;
  Value last;
  Value current;
  const int numFaces = 6;
  IoMod menuWriter{60};

  void validate();
};