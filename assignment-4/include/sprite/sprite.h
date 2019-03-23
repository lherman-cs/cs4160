#pragma once
#include <string>
#include "core/drawable.h"

class Sprite : public Drawable {
 public:
  Sprite(const std::string&);
  Sprite(const Sprite&) = delete;
  virtual ~Sprite() {}
  Sprite& operator=(const Sprite&) = delete;

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const { return image; }

 private:
  const Image* image;
};
