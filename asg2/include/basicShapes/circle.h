#pragma once

#include "renderer/shape.h"

class Circle : public Shape {
 public:
  Circle(int cx, int cy, int r, const std::vector<Shape *> &children);
  void draw(const Renderer &renderer) const;

 private:
  const int cx, cy, r;
};