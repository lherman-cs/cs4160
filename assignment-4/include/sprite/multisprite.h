#pragma once
#include <cmath>
#include <string>
#include <vector>
#include "core/drawable.h"

class MultiSprite : public Drawable {
 public:
  MultiSprite(const std::string&, const std::string& = "");
  MultiSprite(const MultiSprite&) = delete;
  MultiSprite& operator=(const MultiSprite&) = delete;

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return images[currentFrame]; }
  int getScaledWidth() const {
    return getScale() * images[currentFrame]->getWidth();
  }
  int getScaledHeight() const {
    return getScale() * images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

 protected:
  std::vector<Image*> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
};
