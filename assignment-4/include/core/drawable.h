#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "util/image.h"
#include "util/vector2f.h"

// Drawable is an Abstract Base Class (ABC) that specifies the methods
// that derived classes may or must have.
class Drawable {
 public:
  Drawable(const Vector2f& pos) : position(pos) {}

  virtual ~Drawable() {}

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;
  virtual const Image* getImage() const = 0;

  const Vector2f& getPosition() const { return position; }
  void setPosition(const Vector2f& pos) { position = pos; }

 private:
  Vector2f position;
};
