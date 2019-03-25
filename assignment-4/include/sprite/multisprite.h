#pragma once
#include <cmath>
#include <string>
#include <vector>
#include "core/drawable.h"

class MultiSprite : public Drawable {
 public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&) = delete;
  MultiSprite& operator=(const MultiSprite&) = delete;

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const { return images[currentFrame]; }

 protected:
  std::vector<Image*> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
};
