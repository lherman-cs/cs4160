#pragma once
#include <cmath>
#include <string>
#include <vector>
#include "core/interface.h"

class MultiSprite : public Drawable, public Updatable, public Moveable {
 public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&) = delete;
  MultiSprite& operator=(const MultiSprite&) = delete;

  virtual void draw() const;
  virtual void update(Uint32 ticks);

 protected:
  std::vector<Image*> images;

  unsigned currentFrame;
  const unsigned numberOfFrames;
  const unsigned interval;
  int loop;
  unsigned elapsed = 0;
};
