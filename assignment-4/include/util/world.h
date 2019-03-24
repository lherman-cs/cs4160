#pragma once
#include <string>
#include "global/viewport.h"
#include "image.h"

class World {
 public:
  World(const std::string& name, int fact = 1);
  // ~World() { } // The image will be deleted by the FrameFactory
  void update();
  void draw() const;
  World(const World&) = delete;
  World& operator=(const World&) = delete;

 private:
  Image* const image;
  int factor;
  unsigned imageWidth;
  float viewX;
  float viewY;
  const Viewport& view;
};
