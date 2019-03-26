#pragma once
#include <string>
#include "core/interface.h"

class Sprite : public Drawable, public Moveable {
 public:
  Sprite(const std::string&);
  Sprite(const Sprite&) = delete;
  virtual ~Sprite() {}
  Sprite& operator=(const Sprite&) = delete;
  virtual void draw() const;

 private:
  const Image* image;
};
