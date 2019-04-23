#pragma once
#include <SDL2/SDL.h>
#include "util/image.h"

class Drawable {
 public:
  virtual ~Drawable() {}
  virtual void draw() const = 0;
};

class Updatable {
 public:
  virtual ~Updatable() {}
  virtual void update(Uint32 ticks) = 0;
};

class Moveable {
 public:
  virtual ~Moveable() {}
  virtual Vector2f getPosition() const { return position; };
  virtual void setPosition(const Vector2f& newPosition) {
    position = newPosition;
  }

 private:
  Vector2f position{0.0, 0.0};
};

class Trackable : public Drawable, public Updatable, public Moveable {
 public:
  virtual ~Trackable() {}
  virtual const Image* getImage() const = 0;
};

class Reader {
 public:
  virtual ~Reader() {}
  virtual char* read() = 0;
};

class Writer {
 public:
  virtual ~Writer() {}
  virtual void write(const char*) = 0;
};