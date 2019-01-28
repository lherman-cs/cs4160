#pragma once

#include <memory>
#include <vector>
#include "renderer/renderer.h"

class Shape {
 public:
  const int x, y, w, h;
  // children will be drawn in order
  const std::vector<std::shared_ptr<const Shape>> children;

  virtual void draw(const Renderer &renderer) const = 0;

 protected:
  Shape(int x, int y, int w, int h,
        const std::vector<std::shared_ptr<const Shape>> &children);
  virtual ~Shape();
};