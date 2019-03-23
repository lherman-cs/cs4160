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
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel)
      : name(n), position(pos), velocity(vel) {}

  virtual ~Drawable() {}

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;
  virtual const Image* getImage() const = 0;

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n; }

  const Vector2f& getVelocity() const { return velocity; }
  void setVelocity(const Vector2f& vel) { velocity = vel; }
  const Vector2f& getPosition() const { return position; }
  void setPosition(const Vector2f& pos) { position = pos; }

 private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
};
